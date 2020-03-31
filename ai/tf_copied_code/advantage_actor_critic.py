"""
https://github.com/inoryy/tensorflow2-deep-reinforcement-learning/blob/master/a2c.py
http://inoryy.com/post/tensorflow2-deep-reinforcement-learning/
"""

import logging
import numpy as np
import tensorflow as tf
import math


class AgentConfig:
	gamma = 0.99
	value_c = 0.5
	entropy_c = 1e-4


def supplementary_loss(y1, y2):
	return tf.keras.losses.mean_squared_error(y1, y2)


def value_loss(returns, value):
	return AgentConfig.value_c * tf.keras.losses.mean_squared_error(returns, value)


def action_loss(actions_and_advantages, mu_and_sigma):
	if len(actions_and_advantages.shape) == 3:
		return tf.keras.losses.mean_absolute_error(actions_and_advantages, mu_and_sigma)

	mu = tf.squeeze(tf.slice(mu_and_sigma, [0, 0, 0], [-1, -1, 1]), axis=-1)
	sigma = tf.squeeze(tf.slice(mu_and_sigma, [0, 0, 1], [-1, -1, 1]), axis=-1)
	advantages = tf.slice(actions_and_advantages, [0, 0], [-1, 1])
	actions = tf.slice(actions_and_advantages, [0, 1], [-1, -1])

	sigma = tf.math.softplus(sigma)

	Z = (2 * math.pi * sigma ** 2) ** 0.5
	pdf = tf.math.exp(-0.5 * (actions - mu) ** 2 / sigma ** 2) / Z

	badvs = tf.broadcast_to(advantages, pdf.shape)
	# result = -tf.math.log(pdf + 1e-5) * badvs

	# Maybe always
	# If the action was good, badvs > 0, then decrease sigma
	# If the action was bad, badvs < 0, then increase sigma
	result1 = badvs * sigma
	# Move mu in the direction badvs * (actions - mu)
	result2 = pdf * badvs * (actions - mu)
	return tf.math.reduce_sum(
		result2,
		axis=-1
	) + AgentConfig.entropy_c * tf.math.reduce_sum(
		result1,
		axis=-1
	)


def logits_loss(actions_and_advantages, logits):
	# A trick to input actions and advantages through the same API.
	actions, advantages = tf.split(actions_and_advantages, 2, axis=-1)
	# Sparse categorical CE loss obj that supports sample_weight arg on `call()`.
	# `from_logits` argument ensures transformation into normalized probabilities.
	weighted_sparse_ce = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)
	# Policy loss is defined by policy gradients, weighted by advantages.
	# Note: we only calculate the loss on the actions we've actually taken.
	actions = tf.cast(actions, tf.int32)
	policy_loss = weighted_sparse_ce(actions, logits, sample_weight=advantages)
	# Entropy loss can be calculated as cross-entropy over itself.
	probs = tf.nn.softmax(logits)
	entropy_loss = tf.keras.losses.categorical_crossentropy(probs, probs)
	# We want to minimize policy and maximize entropy losses.
	# Here signs are flipped because the optimizer minimizes.
	return policy_loss - AgentConfig.entropy_c * entropy_loss


def returns_advantages(reward, expected_return, dones, next_expected_return):
	# `next_value` is the bootstrap value estimate of the future state (critic).
	returns = np.append(np.zeros_like(reward), np.array([next_expected_return]), axis=-1)
	# Returns are calculated as discounted sum of future rewards.
	for t in reversed(range(reward.shape[0])):
		returns[t] = reward[t] + AgentConfig.gamma * returns[t + 1] * (1 - dones[t])
	returns = returns[:-1]
	# Advantages are equal to returns - baseline (value estimates in our case).
	advantages = returns - expected_return
	return returns, advantages


def action_value(model, obs, step):
	import sys

	tensors = {}
	for k, v in obs.items():
		tensors['input-' + k] = tf.expand_dims(tf.convert_to_tensor(v), axis=0)

	all_output = model.model.predict(tensors)
	# Another way to sample actions:
	#   action = tf.random.categorical(logits, 1)
	# Will become clearer later why we don't use it.

	ret = {
		'value': all_output[0][0, 0],
		'return': all_output[0][0, 0],
	}
	for idx, a in enumerate(model.arch['actions']):
		action = all_output[2 + idx]
		if a['type'] == 'logit':
			ret[a['name']] = tf.random.categorical(action, 1)[0, 0]
		else:
			mu, sigma = tf.split(tf.squeeze(action, axis=0), 2, axis=1)
			shape = (a['n'],)
			mu = tf.reshape(mu, shape),
			sigma = tf.reshape(tf.math.softplus(sigma), shape)

			ret[a['name']] = tf.reshape(
				tf.random.normal(shape, mu, sigma),
				shape
			)

	for idx, h in enumerate(model.arch['supplementary-info']):
		helper = all_output[2 + len(model.arch['actions']) + idx]
		ret[h['name']] = tf.reshape(helper, h['shape'])

	with model.tb_writer.as_default():
		tf.summary.scalar('return', ret['return'], step=step)

	return ret


def train(model, env, batch_size=64, updates=250):
	observations = {
		state['name']: np.empty((batch_size,) + state['shape'])
		for state in model.arch['state']
	}
	expected_return = np.empty((batch_size,), dtype=np.float32)
	rewards = np.empty((batch_size,), dtype=np.float32)
	actions = {
		action['name']: np.empty((batch_size,), dtype=np.int32) if action['type'] == 'logit' else
		np.empty((batch_size,) + (action['n'],), dtype=np.float32)
		for action in model.arch['actions']
	}
	dones = np.empty((batch_size,), dtype=np.bool)
	sup_true = {
		sup['name']: np.empty((batch_size,) + sup['shape'])
		for sup in model.arch['supplementary-info']
	}

	training_cum_rewards = [0.0]
	next_observations = env.reset()
	for update in range(updates):
		for step in range(batch_size):
			for state_name, state_value in next_observations.items():
				observations[state_name][step] = state_value

			model_output = action_value(model, next_observations, update * batch_size + step)
			expected_return[step] = model_output['return']
			for action in model.arch['actions']:
				actions[action['name']][step] = model_output[action['name']]

			next_observations, rewards[step], dones[step], prev_sup = env.step(model_output)
			for sup in model.arch['supplementary-info']:
				sup_true[sup['name']][step] = prev_sup[sup['name']]

			training_cum_rewards[-1] += rewards[step]
			if dones[step]:
				training_cum_rewards.append(0.0)
				next_observations = env.reset()
				logging.info("Episode: %03d, Reward: %03f" % (len(training_cum_rewards) - 1, training_cum_rewards[-2]))

		next_expected_return = action_value(model, next_observations, update)['return']

		returns, advs = returns_advantages(rewards, expected_return, dones, next_expected_return)

		training_output = [
			np.random.random((batch_size,)),  # value
			returns,
		]

		for action in model.arch['actions']:
			if action['type'] == 'logit':
				training_output.append(np.concatenate([actions[action['name']][:, None], advs[:, None]], axis=-1))
			else:
				training_output.append(np.concatenate([advs[:, None], actions[action['name']]], axis=1))

		for supp in model.arch['supplementary-info']:
			training_output.append(sup_true[supp['name']])

		# Performs a full training step on the collected batch.
		# Note: no need to mess around with gradients, Keras API handles it.
		losses = model.model.train_on_batch(
			{'input-' + k: v for k, v in observations.items()},
			training_output
		)
		logging.debug("[%d/%d] Losses: %s" % (update + 1, updates, losses))

	model.rewards_history += training_cum_rewards

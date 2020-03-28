"""
https://github.com/inoryy/tensorflow2-deep-reinforcement-learning/blob/master/a2c.py
http://inoryy.com/post/tensorflow2-deep-reinforcement-learning/
"""

import logging
import numpy as np
import tensorflow as tf


class AgentConfig:
	gamma = 0.99
	value_c = 0.5
	entropy_c = 1e-4


def value_loss(returns, value):
	# Value loss is typically MSE between value estimates and returns.
	return AgentConfig.value_c * tf.keras.losses.mean_squared_error(returns, value)


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
	entropy_loss = tf.keras.losses.categorical_crossentropy(probs, probs)  # Not sure if this is right
	# We want to minimize policy and maximize entropy losses.
	# Here signs are flipped because the optimizer minimizes.
	return policy_loss - AgentConfig.entropy_c * entropy_loss


def returns_advantages(reward, expected_return, dones, next_expected_return):
	# `next_value` is the bootstrap value estimate of the future state (critic).
	returns = np.append(np.zeros_like(reward), next_expected_return, axis=-1)
	# Returns are calculated as discounted sum of future rewards.
	for t in reversed(range(reward.shape[0])):
		returns[t] = reward[t] + AgentConfig.gamma * returns[t + 1] * (1 - dones[t])
	returns = returns[:-1]
	# Advantages are equal to returns - baseline (value estimates in our case).
	advantages = returns - expected_return
	return returns, advantages


def action_value(model, obs):
	# Executes `call()` under the hood.
	all_outputs = model.predict_on_batch(obs)
	# Another way to sample actions:
	#   action = tf.random.categorical(logits, 1)
	# Will become clearer later why we don't use it.
	return [
		np.squeeze(all_outputs[0], axis=-1),  # value
		np.squeeze(all_outputs[1], axis=-1),  # return
	] + [
		tf.squeeze(tf.squeeze(tf.random.categorical(action, 1) if a['type'] == 'logit' else action, axis=-1), axis=-1)
		for action, a in zip(all_outputs[2:], model.actions)
	]


def train(model, env, batch_size=64, updates=250):
	observations = [
		np.empty((batch_size,) + state['shape'])
		for state in model.states
	]
	expected_return = np.empty((batch_size,), dtype=np.float32)
	rewards = np.empty((batch_size,), dtype=np.float32)
	actions = [
		np.empty((batch_size,), dtype=np.int32) if action['type'] == 'logit' else
		np.empty((batch_size,) + action['shape'], dtype=np.float32)
		for action in model.actions
	]
	dones = np.empty((batch_size,), dtype=np.bool)

	training_cum_rewards = [0.0]
	next_observations = env.reset()
	for update in range(updates):
		for step in range(batch_size):
			for observation, next_observation in zip(observations, next_observations):
				observation[step] = next_observation
			model_output = action_value(model, [o[None, :] for o in next_observations])
			expected_return[step] = model_output[1]
			for idx in range(len(actions)):
				actions[idx][step] = model_output[2 + idx]

			next_observations, rewards[step], dones[step] = env.step(model_output[2:])

			training_cum_rewards[-1] += rewards[step]
			if dones[step]:
				training_cum_rewards.append(0.0)
				next_observations = env.reset()
				logging.info("Episode: %03d, Reward: %03f" % (len(training_cum_rewards) - 1, training_cum_rewards[-2]))

		next_expected_return = action_value(model, [o[None, :] for o in next_observations])[1]

		returns, advs = returns_advantages(rewards, expected_return, dones, next_expected_return)

		training_output = [
			np.random.random((batch_size,)),  # value
			returns,  # return
		] + [
			np.concatenate([action[:, None], advs[:, None]], axis=-1)
			for action in actions
		]

		# Performs a full training step on the collected batch.
		# Note: no need to mess around with gradients, Keras API handles it.
		losses = model.train_on_batch(observations, training_output)
		logging.debug("[%d/%d] Losses: %s" % (update + 1, updates, losses))

	model.rewards_history += training_cum_rewards

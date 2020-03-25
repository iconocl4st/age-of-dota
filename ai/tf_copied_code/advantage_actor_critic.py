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


def returns_advantages(rewards, dones, values, next_value):
	# `next_value` is the bootstrap value estimate of the future state (critic).
	returns = np.append(np.zeros_like(rewards), next_value, axis=-1)
	# Returns are calculated as discounted sum of future rewards.
	for t in reversed(range(rewards.shape[0])):
		returns[t] = rewards[t] + AgentConfig.gamma * returns[t + 1] * (1 - dones[t])
	returns = returns[:-1]
	# Advantages are equal to returns - baseline (value estimates in our case).
	advantages = returns - values
	return returns, advantages


def action_value(model, obs):
	# Executes `call()` under the hood.
	logits, value = model.predict_on_batch(obs)
	action = model.dist.predict_on_batch(logits)
	# Another way to sample actions:
	#   action = tf.random.categorical(logits, 1)
	# Will become clearer later why we don't use it.
	return np.squeeze(action, axis=-1), np.squeeze(value, axis=-1)


def train(model, env, batch_size=64, updates=250):
	actions = np.empty((batch_size,), dtype=np.int32)
	rewards = np.empty((batch_size,), dtype=np.float32)
	dones = np.empty((batch_size,), dtype=np.bool)
	values = np.empty((batch_size,), dtype=np.float32)

	observations = np.empty((batch_size,) + env.observation_space.shape)
	training_rewards = [0.0]
	next_obs = env.reset()
	for update in range(updates):
		for step in range(batch_size):
			observations[step] = next_obs.copy()
			actions[step], values[step] = action_value(model, next_obs[None, :])
			next_obs, rewards[step], dones[step], _ = env.step(actions[step])

			training_rewards[-1] += rewards[step]
			if dones[step]:
				training_rewards.append(0.0)
				next_obs = env.reset()
				logging.info("Episode: %03d, Reward: %03f" % (len(training_rewards) - 1, training_rewards[-2]))

		_, next_value = action_value(model, next_obs[None, :])
		returns, advs = returns_advantages(rewards, dones, values, next_value)
		# A trick to input actions and advantages through same API.
		acts_and_advs = np.concatenate([actions[:, None], advs[:, None]], axis=-1)
		# Performs a full training step on the collected batch.
		# Note: no need to mess around with gradients, Keras API handles it.
		losses = model.train_on_batch(observations, [acts_and_advs, returns])
		logging.debug("[%d/%d] Losses: %s" % (update + 1, updates, losses))

	model.rewards_history += training_rewards

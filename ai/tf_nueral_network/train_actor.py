from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

import numpy as np
from app.assignment import Assignment
from .nn_building_common import get_n_states_input, get_n_actions_input
from .policy_function import PolicyHolder
from .simple_critic import SimpleCriticFunction


class LossFunction:
	LossFunction = None


def create_mock_action_output(num_states):
	ret = []
	for policy_output in PolicyHolder.get_policy_outputs():
		layer = None
		for cLayer in PolicyHolder.get_policy().layers:
			if 'out_' + policy_output in cLayer.name:
				layer = cLayer
				break
		shape = layer.output_shape
		if isinstance(shape, list):
			shape = shape[0]
		if shape is None:
			print('could not find ' + policy_output)
		if shape[0] is not None:
			raise Exception()
		actual_shape = tuple(s if s is not None else num_states for s in shape)
		ret.append(np.random.random(actual_shape))
	return ret


def loss_function(y_pred, y_action):
	print(y_pred.shape)
	return tf.math.reduce_sum(
		tf.math.square(
			tf.math.subtract(
				tf.cast(y_pred, dtype=tf.float64),
				tf.cast(y_action, dtype=tf.float64)
			)
		)
	)


def create_loss_function():
	policy = PolicyHolder.get_policy()
	critic = SimpleCriticFunction.get_critic()


def train_actor(game_states, actions):
	assignments = [Assignment(game['player-number']) for game in game_states]

	state_input = get_n_states_input(game_states, assignments)
	actions_input = get_n_actions_input(actions, assignments)
	mock_output = create_mock_action_output(len(game_states))

	policy = PolicyHolder.get_policy()
	policy.add_loss(loss_function)
	policy.compile(optimizer='adam')
	inputs = {**state_input, **actions_input}
	policy.fit(state_input, mock_output, epochs=100)
	policy.save_weights(PolicyHolder.get_save_path())


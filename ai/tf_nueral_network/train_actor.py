from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

import numpy as np
from app.assignment import Assignment
from app.encoding_limits import Actions, NumpyLimits, Resources
from .nn_building_common import get_n_states_input, get_n_actions_input
from .policy_function import PolicyHolder
from .simple_critic import SimpleCriticFunction
from app.game_state import StateIndices, get_training_dimensions


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


def get_policy_output_tensors():
	ret = {}
	for policy_output in PolicyHolder.get_policy_outputs():
		layer = None
		for cLayer in PolicyHolder.get_policy().layers:
			if 'out_' + policy_output in cLayer.name:
				layer = cLayer
				break
		ret[policy_output] = layer
	return ret


def get_critic_input_layers():
	ret = {}
	critic = SimpleCriticFunction.get_critic()
	training_dimensions = get_training_dimensions()
	for action_obj in ['state', 'actions']:
		for input_key in training_dimensions[action_obj]['inputs']:
			for layer in critic.layers:
				if layer.name == 'flattened_' + input_key:
					ret[input_key] = layer
					break
	return ret


def get_policy_input_layers():
	ret = {}
	critic = PolicyHolder.get_policy()
	training_dimensions = get_training_dimensions()
	for input_key in training_dimensions['state']['inputs']:
		for layer in critic.layers:
			if layer.name == input_key:
				ret[input_key] = layer
				break
	return ret


def take_index3(x, idx, training_size):
	return tf.slice(
		x,
		begin=(0, 0, idx),
		size=(training_size, NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, 1)
	)


def loss_function(size):
	critic = SimpleCriticFunction.get_critic()

	# TODO fix dtypes...

	# TODO: not used...
	loss_stubs = PolicyHolder.get_loss_stubs()
	critic_inputs = get_critic_input_layers()
	policy_input = get_policy_input_layers()

	policy_output = get_policy_output_tensors()
	which_action = policy_output['which-action'].output
	movement = policy_output['movement'].output
	strafe_is_left = tf.expand_dims(policy_output['strafe-direction'].output, axis=2)
	take_action = policy_output['change-action'].output

	if False:
		collect_resource = tf.one_hot(policy_output['collect-resources'].output, Resources.NUM_RESOURCES)
		deposit_resource = tf.one_hot(policy_output['deposit-resources'].output, Resources.NUM_RESOURCES)
	else:
		collect_resource = tf.minimum(
			tf.expand_dims(policy_output['collect-resources'].output, axis=2),
			tf.constant(0, dtype=tf.int64, shape=(size, NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, 1))
		)
		deposit_resource = tf.minimum(
			tf.expand_dims(policy_output['deposit-resources'].output, axis=2),
			tf.constant(0, dtype=tf.int64, shape=(size, NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, 1))
		)

	which_action = tf.one_hot(which_action, Actions.NUM_ACTIONS)
	is_idle = take_index3(which_action, Actions.IDLE, size)
	is_move = take_index3(which_action, Actions.MOVE, size)
	is_collect = take_index3(which_action, Actions.COLLECT, size)
	is_deposit = take_index3(which_action, Actions.DEPOSIT, size)
	is_attack = take_index3(which_action, Actions.ATTACK, size)
	is_strafe = take_index3(which_action, Actions.STRAFE, size)

	movement_dd = tf.slice(movement, (0, 0, 0), (size, NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, 1))
	movement_x = tf.slice(movement, (0, 0, 1), (size, NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, 1))
	movement_y = tf.slice(movement, (0, 0, 2), (size, NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, 1))

	action = tf.concat([
		tf.cast(is_idle, dtype=tf.float64),
		tf.cast(is_collect, dtype=tf.float64),
		tf.cast(collect_resource, dtype=tf.float64),
		tf.cast(is_deposit, dtype=tf.float64),
		tf.cast(deposit_resource, dtype=tf.float64),
		tf.cast(is_strafe, dtype=tf.float64),
		tf.cast(strafe_is_left, dtype=tf.float64),
		tf.cast(is_attack, dtype=tf.float64),
		tf.cast(is_move, dtype=tf.float64),
		tf.cast(movement_dd, dtype=tf.float64),
		tf.cast(movement_x, dtype=tf.float64),
		tf.cast(movement_y, dtype=tf.float64)
	], axis=2)

	return lambda: tf.reduce_sum(action)

	# cant_take_action = tf.repeat(tf.expand_dims(take_action, axis=2), StateIndices.ACTION_IDX, axis=2)
	# actions = tf.math.multiply(action, tf.cast(cant_take_action, dtype=tf.float64))
	#
	# flattened_entities = critic_inputs['entities'](policy_input['entities'].output)
	# flattened_projectiles = critic_inputs['projectiles'](policy_input['projectiles'].output)
	# flattened_game_times = critic_inputs['game-time'](policy_input['game-time'].output)
	# flattened_actions = critic_inputs['actions'](actions)
	#
	# # TODO: No magic index
	# # TODO: They might not even be the  right order...
	# x = critic.layers[8]([flattened_entities, flattened_projectiles, flattened_game_times, flattened_actions])
	#
	# for layer in critic.layers:
	# 	if 'the_meat' in layer.name or 'the_output' == layer.name:
	# 		x = layer(x)
	#
	# # TODO, why is x an int?????????
	# x = tf.subtract(tf.constant(1.0, dtype=tf.float64, shape=(size, 1)), tf.cast(x, dtype=tf.float64))
	# return lambda: x


def train_actor(game_states, actions):
	assignments = [Assignment(game['player-number']) for game in game_states]

	state_input = get_n_states_input(game_states, assignments)
	actions_input = get_n_actions_input(actions, assignments)
	mock_output = create_mock_action_output(len(game_states))

	policy = PolicyHolder.get_policy()
	policy.add_loss(loss_function(len(game_states)))
	policy.compile(optimizer='adam')
	inputs = {**state_input, **actions_input}
	policy.fit(inputs, epochs=100)
	policy.save_weights(PolicyHolder.get_save_path())


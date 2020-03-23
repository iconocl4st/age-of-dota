from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

import numpy as np

from app.assignment import Assignment
from app.game_state import get_training_dimensions, actions_to_numpy
from app.game_state import game_state_to_numpy


def add_dense_layers(width, height, nn):
	if isinstance(width, list):
		for w in width:
			for i in range(height):
				nn = tf.keras.layers.Dense(w, activation='relu')(nn)
				nn = tf.keras.layers.Dropout(0.2)(nn)
	else:
		for i in range(height):
			nn = tf.keras.layers.Dense(width, activation='relu')(nn)
			nn = tf.keras.layers.Dropout(0.2)(nn)
	return nn


def get_random_player(state):
	return np.random.randint(1, len(state['entities-by-player']))


def initialize_n_input(input_obj, n):
	input_dict = {}
	for input_key in input_obj['inputs']:
		input_dict[input_key] = np.zeros(tuple(
			[n] + [i for i in input_obj[input_key]]
		))
	return input_dict


def get_n_states_input(game_states, assignments=None):
	num_games = len(game_states)
	training_dimensions = get_training_dimensions()
	state_input = initialize_n_input(training_dimensions['state'], num_games)
	for game_idx, game_state in enumerate(game_states):
		assignment = Assignment(game_state['player-number']) if assignments is None else assignments[game_idx]
		game_encoding = game_state_to_numpy(game_state, assignment=assignment)
		for input_key in training_dimensions['state']['inputs']:
			state_input[input_key][game_idx] = game_encoding[input_key]
	return state_input


def get_n_actions_input(actions, assignments):
	num_games = len(actions)
	training_dimensions = get_training_dimensions()
	actions_input = initialize_n_input(training_dimensions['actions'], num_games)
	for idx in range(num_games):
		actions_input['actions'][idx] = actions_to_numpy(actions[idx], assignments[idx])['actions']
	return actions_input


def get_nn_inputs(input_obj):
	inputs = []
	input_ends = []
	for input_key in input_obj['inputs']:
		input_layer = tf.keras.Input(
			shape=tuple([i for i in input_obj[input_key]]),
			name=input_key
		)
		inputs.append(input_layer)
		input_ends.append(tf.keras.layers.Flatten()(input_layer))
	return inputs, input_ends

from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

from .value_function import ValueHolder
from .simple_critic import SimpleCriticFunction
import numpy as np
from app.game_state import get_training_dimensions
from app.game_state import actions_to_numpy
from app.assignment import Assignment
from .nn_building_common import get_n_states_input, get_n_actions_input
from .nn_building_common import initialize_n_input


def remove_none(next_states, assignments):
	game_idx_to_value_idx = {}
	states = []
	asss = []
	for i in range(len(next_states)):
		if next_states[i] is None:
			continue
		game_idx_to_value_idx[i] = len(states)
		states.append(next_states[i])
		asss.append(assignments[i])
	return states, asss, game_idx_to_value_idx


def train_simple_critic(game_states, actions, next_states, won):
	num_games = len(game_states)
	training_dimensions = get_training_dimensions()

	assignments = [Assignment(game['player-number']) for game in game_states]

	rstates, rassigns, idx_to_vidx = remove_none(next_states, assignments)

	rtensors = get_n_states_input(rstates, rassigns)
	model = ValueHolder.get_value()
	value_predictions = model.predict(rtensors)

	values = np.zeros(num_games, dtype=float)
	for idx in range(num_games):
		if idx in idx_to_vidx:
			values[idx] = value_predictions[idx_to_vidx[idx]]
		else:
			values[idx] = 1.0 if won[idx] else 0.0

	state_input = get_n_states_input(game_states, assignments)
	actions_input = get_n_actions_input(actions, assignments)

	critic = SimpleCriticFunction.get_critic()
	critic.compile(
		optimizer='adam',
		loss=tf.keras.losses.BinaryCrossentropy()
	)
	inputs = {**state_input, **actions_input}
	critic.fit(inputs, values, epochs=100)
	critic.save_weights(SimpleCriticFunction.get_save_path())

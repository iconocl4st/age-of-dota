from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

from .value_function import ValueHolder
import numpy as np
from app.game_state import get_training_dimensions
from app.game_state import game_state_to_numpy


def train_value_function(game_states, player_numbers, won):
    num_games = len(game_states)
    training_dimensions = get_training_dimensions()

    training_input = {}
    training_output = {"won": np.zeros((num_games, 2))}
    for input in training_dimensions['state']['inputs']:
        training_input[input] = np.zeros(tuple(
            [num_games] + [i for i in training_dimensions['state'][input]]
        ))

    for game_idx, game_state in enumerate(game_states):
        game_encoding = game_state_to_numpy(game_state, player_numbers[game_idx])
        for input in training_dimensions['state']['inputs']:
            training_input[input][game_idx] = game_encoding[input]
        if won[game_idx]:
            training_output['won'][game_idx][0] = 1
        else:
            training_output['won'][game_idx][1] = 1

    model = ValueHolder.get_value()

    model.compile(
        optimizer='adam',
        loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
        metrics=['accuracy']
    )

    model.fit(training_input, training_output['won'][:, 0], epochs=100)

    model.save_weights(ValueHolder.get_save_path())

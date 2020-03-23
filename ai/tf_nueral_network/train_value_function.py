from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

from .value_function import ValueHolder
import numpy as np
from .nn_building_common import get_n_states_input


def train_value_function(game_states, won):
    state_input = get_n_states_input(game_states)

    training_output = np.array([1.0 if w else 0.0 for w in won])

    model = ValueHolder.get_value()
    model.compile(
        optimizer='adam',
        loss='binary_crossentropy',
        metrics=['accuracy']
    )
    model.fit(state_input, training_output, epochs=20)
    model.save_weights(ValueHolder.get_save_path())

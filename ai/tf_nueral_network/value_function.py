from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

import numpy as np
from app.game_state import get_training_dimensions
from app.encoding_limits import NumpyLimits
from app.encoding_limits import Actions
from app.encoding_limits import Resources

class ValueHolder:
    _ValueFunction = None

    @classmethod
    def _ensure_initialized(cls):
        if cls._ValueFunction is not None:
            return
        cls._ValueFunction = construct_value_function()

    @classmethod
    def get_value(cls):
        cls._ensure_initialized()
        return cls._ValueFunction


def construct_value_function():
    width = 128
    height = 2

    training_dimensions = get_training_dimensions()
    inputs = []
    input_ends = []
    for input in training_dimensions['state']['inputs']:
        input_layer = tf.keras.Input(
            shape=tuple([i for i in training_dimensions['state'][input]]),
            name=input
        )
        inputs.append(input_layer)
        input_ends.append(tf.keras.layers.Flatten()(input_layer))

    x = tf.keras.layers.Concatenate()(input_ends)

    for i in range(height):
        x = tf.keras.layers.Dense(width, activation='relu')(x)
        x = tf.keras.layers.Dropout(0.2)(x)
    x = tf.keras.layers.Dense(2, activation='softmax')(x)

    model = tf.keras.Model(inputs=inputs, outputs=x, name='ai_value_function')

    tf.keras.utils.plot_model(model, 'saved_networks/value_function.png', show_shapes=True)

    return model

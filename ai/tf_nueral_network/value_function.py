from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

from .set_memory import MemorySetter
import os
from app.game_state import get_training_dimensions


class ValueHolder:
    _ValueFunction = None

    @classmethod
    def _ensure_initialized(cls):
        if cls._ValueFunction is not None:
            return
        MemorySetter.set_memory_usage()
        cls._ValueFunction = construct_value_function()

        if os.path.exists(ValueHolder.get_save_path() + '.index'):
            cls._ValueFunction.load_weights(ValueHolder.get_save_path())

        checkpoint_path = "output/saved_networks/value-cp-{epoch:04d}.ckpt"

    @classmethod
    def get_value(cls):
        cls._ensure_initialized()
        return cls._ValueFunction

    @staticmethod
    def get_save_path():
        return './output/saved_networks/value_function'


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

    tf.keras.utils.plot_model(model, 'output/saved_networks/value_function.png', show_shapes=True)

    return model

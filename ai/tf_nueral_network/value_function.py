from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

from .set_memory import MemorySetter
import os
from app.game_state import get_training_dimensions
from .nn_building_common import add_dense_layers
from .nn_building_common import get_nn_inputs


class ValueHolder:
    _ValueFunction = None

    @classmethod
    def _ensure_initialized(cls):
        if cls._ValueFunction is not None:
            return
        MemorySetter.set_memory_usage()
        cls._ValueFunction = construct_value_function()

        if os.path.exists(ValueHolder.get_save_path() + '.index'):
            try:
                cls._ValueFunction.load_weights(ValueHolder.get_save_path())
            except:
                print('Could not load previous model')

        checkpoint_path = "output/saved_networks/value-cp-{epoch:04d}.ckpt"

    @classmethod
    def get_value(cls):
        cls._ensure_initialized()
        return cls._ValueFunction

    @staticmethod
    def get_save_path():
        return './output/saved_networks/value_function'


def construct_value_function():
    training_dimensions = get_training_dimensions()
    inputs, input_ends = get_nn_inputs(training_dimensions['state'])

    x = tf.keras.layers.Concatenate()(input_ends)
    x = add_dense_layers([1024, 512, 128], 1, x)
    x = tf.keras.layers.Dense(1, activation='sigmoid')(x)

    model = tf.keras.Model(inputs=inputs, outputs=x, name='ai_value_function')
    tf.keras.utils.plot_model(model, 'output/saved_networks/value_function.png', rankdir='LR', show_shapes=True)
    return model

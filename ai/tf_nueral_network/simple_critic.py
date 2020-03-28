from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
import os

from app.game_state import get_training_dimensions
from .nn_building_common import add_dense_layers
from .nn_building_common import get_nn_inputs
from .set_memory import MemorySetter


class SimpleCriticFunction:
    _CriticFunction = None

    @classmethod
    def _ensure_initialized(cls):
        if cls._CriticFunction is not None:
            return

        MemorySetter.set_memory_usage()
        cls._CriticFunction = construct_critic()

        if os.path.exists(SimpleCriticFunction.get_save_path() + '.index'):
            try:
                cls._CriticFunction.load_weights(SimpleCriticFunction.get_save_path())
            except:
                print('could not load previous critic')

    @classmethod
    def get_critic(cls):
        cls._ensure_initialized()
        return cls._CriticFunction

    @staticmethod
    def get_save_path():
        return './output/saved_networks/critic_function'


def construct_critic():
    training_dimensions = get_training_dimensions()

    state_inputs, state_input_ends = get_nn_inputs(training_dimensions['state'])
    action_inputs, action_input_ends = get_nn_inputs(training_dimensions['actions'])

    x = tf.keras.layers.Concatenate()(state_input_ends + action_input_ends)
    x = add_dense_layers(128, 3, x, name='critic')
    x = tf.keras.layers.Dense(1, activation='sigmoid', name='the_output')(x)

    model = tf.keras.Model(inputs=state_inputs + action_inputs, outputs=x, name='ai_critic_function')
    tf.keras.utils.plot_model(model, 'output/saved_networks/critic_function.png', rankdir='LR', show_shapes=True)
    return model


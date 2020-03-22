from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
import os

from app.game_state import get_training_dimensions
from app.encoding_limits import NumpyLimits
from app.encoding_limits import Actions
from app.encoding_limits import Resources
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
            cls._CriticFunction.load_weights(SimpleCriticFunction.get_save_path())

    @classmethod
    def get_policy(cls):
        cls._ensure_initialized()
        return cls._CriticFunction

    @staticmethod
    def get_save_path():
        return './output/saved_networks/critic_function'


def construct_critic():
    width = 128
    height = 3

    training_dimensions = get_training_dimensions()
    inputs = []
    input_ends = []
    for input in [
        training_dimensions['state']['inputs'] +
        training_dimensions['state']['actions']
    ]:
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

    last_layer = x

    outputs = {
        'which-action': create_categorical_output(last_layer, Actions.NUM_ACTIONS),
        'movement': create_linear_output(last_layer, 3),
        'collect-resources': create_categorical_output(last_layer, Resources.NUM_RESOURCES),
        'deposit-resources': create_categorical_output(last_layer, Resources.NUM_RESOURCES),
        'strafe-direction': create_categorical_output(last_layer, 2),
        'change-action': create_categorical_output(last_layer, 2),
    }

    model = tf.keras.Model(inputs=inputs, outputs=[o for o in outputs.values()], name='ai_policy_function')
    tf.keras.utils.plot_model(model, 'output/saved_networks/policy_function.png', show_shapes=True)
    return model, [k for k in outputs.keys()]


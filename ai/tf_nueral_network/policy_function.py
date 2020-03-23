from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
import os

from app.game_state import get_training_dimensions
from app.encoding_limits import NumpyLimits
from app.encoding_limits import Actions
from app.encoding_limits import Resources
from .nn_building_common import add_dense_layers
from .nn_building_common import get_nn_inputs

from .set_memory import MemorySetter


class PolicyHolder:
    _PolicyFunction = None
    _PolicyOutputs = None

    @classmethod
    def _ensure_initialized(cls):
        if cls._PolicyFunction is not None:
            return

        MemorySetter.set_memory_usage()
        cls._PolicyFunction, cls._PolicyOutputs = construct_policy()

        if os.path.exists(PolicyHolder.get_save_path() + '.index'):
            try:
                cls._PolicyFunction.load_weights(PolicyHolder.get_save_path())
            except:
                print('could not load previous policy')

    @classmethod
    def get_policy(cls):
        cls._ensure_initialized()
        return cls._PolicyFunction

    @classmethod
    def get_policy_outputs(cls):
        cls._ensure_initialized()
        return cls._PolicyOutputs

    @staticmethod
    def get_save_path():
        return './output/saved_networks/policy_function'


def create_categorical_output(last_layer, dim, name):
    x = last_layer
    x = tf.keras.layers.Dense(
        NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER * dim,
        activation='relu'
    )(x)
    x = tf.keras.layers.Reshape((NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, dim))(x)
    x = tf.nn.softmax(logits=x, axis=-1)
    x = tf.math.argmax(x, axis=-1, name='out_' + name)
    return x


def create_linear_output(last_layer, dim, name):
    x = last_layer
    x = tf.keras.layers.Dense(
        NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER * dim,
        activation='linear'
    )(x)
    x = tf.keras.layers.Reshape((NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER, dim), name='out_' + name)(x)
    return x


def construct_policy():
    training_dimensions = get_training_dimensions()
    inputs, input_ends = get_nn_inputs(training_dimensions['state'])

    action_inputs, action_ends = get_nn_inputs(training_dimensions['actions'])

    x = tf.keras.layers.Concatenate()(input_ends)

    x = add_dense_layers(128, 3, x)

    last_layer = x

    outputs = {
        name: method(last_layer, size, name)
        for name, method, size in [
            ('which-action', create_categorical_output, Actions.NUM_ACTIONS),
            ('movement', create_linear_output, 3),
            ('collect-resources', create_categorical_output, Resources.NUM_RESOURCES),
            ('deposit-resources', create_categorical_output, Resources.NUM_RESOURCES),
            ('strafe-direction', create_categorical_output, 2),
            ('change-action', create_categorical_output, 2),
        ]
    }

    model = tf.keras.Model(inputs=inputs + action_inputs, outputs=[o for o in outputs.values()], name='ai_policy_function')
    tf.keras.utils.plot_model(model, 'output/saved_networks/policy_function.png', rankdir='LR', show_shapes=True)
    return model, [k for k in outputs.keys()]


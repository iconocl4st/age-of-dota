from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
import numpy as np

from .policy_function import PolicyHolder
from app.game_state import get_training_dimensions
from app.game_state import game_state_to_numpy
from app.encoding_limits import Actions
from app.assignment import Assignment
from .set_memory import MemorySetter


def create_action_zeros():
    training_dimensions = get_training_dimensions()['actions']
    return {
        input_key: np.zeros(training_dimensions[input_key])
        for input_key in training_dimensions['inputs']
    }


def np_to_tensor(obj, name):
    MemorySetter.set_memory_usage()
    ret = tf.convert_to_tensor(obj[name], name=name, dtype=tf.float64)
    ret = tf.expand_dims(ret, axis=0)
    return ret


def create_action_tensor():
    numpy_mat = create_action_zeros()
    return {key: np_to_tensor(numpy_mat, key) for key in numpy_mat.keys()}


class MockActionsForLoss:
    MOCK_ACTIONS = create_action_zeros()
    ACTION_TENSOR = create_action_tensor()

    @staticmethod
    def mock_actions(num_reps):
        return {
            key: np.repeat(value[np.newaxis, :], num_reps, axis=0)
            for key, value in MockActionsForLoss.MOCK_ACTIONS.items()
        }


def parse_actions(output, game_state, assignments, player_number):
    actions = {}

    for entity_id in game_state['entities-by-player'][player_number].keys():
        entity_idx = assignments.get_entity_index(0, entity_id)
        if output['change-action'][0, entity_idx] == 0:
            continue
        action_obj = {'action-type': int(output['which-action'][0, entity_idx])}
        if action_obj['action-type'] == Actions.IDLE:
            action_obj['action-args'] = {}
        elif action_obj['action-type'] == Actions.MOVE:
            action_obj['action-args'] = {
                'desired-orientation': float(output['movement'][0, entity_idx, 2]),
                'path': [{
                    'x': float(output['movement'][0, entity_idx, 0]),
                    'y': float(output['movement'][0, entity_idx, 1]),
                }]
            }
        elif action_obj['action-type'] == Actions.COLLECT:
            action_obj['action-args'] = {
                'resource': int(output['collect-resources'][0, entity_idx])
            }
        elif action_obj['action-type'] == Actions.DEPOSIT:
            action_obj['action-args'] = {
                'resource': int(output['deposit-resources'][0, entity_idx])
            }
        elif action_obj['action-type'] == Actions.ATTACK:
            action_obj['action-args'] = {}
        elif action_obj['action-type'] == Actions.STRAFE:
            action_obj['action-args'] = {
                'is-left': bool(output['change-action'][0, entity_idx] == 1),
            }
        else:
            raise Exception("Invalid action")
        actions[entity_id] = action_obj
    return actions


def evaluate_policy(game_state, assignment=None):
    if assignment is None:
        assignment = Assignment(game_state['player-number'])

    state_data = game_state_to_numpy(
        state=game_state,
        assignment=assignment
    )
    state_tensors = {
        key: np_to_tensor(state_data, key)
        for key in state_data.keys()
    }
    predictions = PolicyHolder.get_policy().predict({
        **state_tensors,
        **MockActionsForLoss.ACTION_TENSOR
    })
    return parse_actions(
        {
            key: prediction
            for prediction, key in zip(predictions, PolicyHolder.get_policy_outputs())
        },
        game_state,
        assignment,
        game_state['player-number']
    )


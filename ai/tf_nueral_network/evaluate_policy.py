from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

from .policy_function import PolicyHolder
from app.game_state import game_state_to_numpy
from app.encoding_limits import Actions
from app.assignment import Assignment


def np_to_tensor(obj, name):
    ret = tf.convert_to_tensor(obj[name], name=name, dtype=tf.float64)
    ret = tf.expand_dims(ret, axis=0)
    return ret


def parse_actions(output, game_state, assignments, player_number):
    player_index = assignments.get_player_index(player_number, player_number)

    actions = {}

    for idx, entity in enumerate(game_state['entities-by-player'][player_number].values()):
        entity_idx = assignments.entity_mapping[player_index, idx]
        if output['change-action'][0, entity_idx] == 0:
            continue
        action_obj = { 'action-type': int(output['which-action'][0, entity_idx]) }
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
        actions[entity['id']] = action_obj
    return actions


def evaluate_policy(game_state, player_number=None, assignment=None):
    if assignment is None:
        assignment = Assignment()
    if player_number is None:
        player_number = game_state['player-number']

    state_data = game_state_to_numpy(
        state=game_state,
        player_number=player_number,
        assignment=assignment
    )
    predictions = PolicyHolder.get_policy().predict({
        key: np_to_tensor(state_data, key)
        for key in state_data.keys()
    })
    return parse_actions(
        {
            key: prediction
            for prediction, key in zip(predictions, PolicyHolder.get_policy_outputs())
        },
        game_state,
        assignment,
        player_number
    )


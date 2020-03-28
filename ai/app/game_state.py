
import numpy as np
from .encoding_limits import Actions
from .encoding_limits import NumpyLimits
from .assignment import Assignment


class StateIndices:
    ACTION_IDX = 0
    # closely tied to the train actor file
    ACTION_IS_IDLE = ACTION_IDX; ACTION_IDX += 1
    ACTION_IS_COLLECT = ACTION_IDX; ACTION_IDX += 1
    ACTION_COLLECT_RESOURCE = ACTION_IDX; ACTION_IDX += 1
    ACTION_IS_DEPOSIT = ACTION_IDX; ACTION_IDX += 1
    ACTION_DEPOSIT_RESOURCE = ACTION_IDX; ACTION_IDX += 1
    ACTION_IS_STRAFE = ACTION_IDX; ACTION_IDX += 1
    ACTION_STRAFE_IS_LEFT = ACTION_IDX; ACTION_IDX += 1
    ACTION_IS_ATTACK = ACTION_IDX; ACTION_IDX += 1
    ACTION_IS_MOVEMENT = ACTION_IDX; ACTION_IDX += 1
    ACTION_MOVEMENT_DESIRED_ORIENTATION = ACTION_IDX; ACTION_IDX += 1
    ACTION_MOVEMENT_X = ACTION_IDX; ACTION_IDX += 1
    ACTION_MOVEMENT_Y = ACTION_IDX; ACTION_IDX += 1

    # ACTION_TRANSFER_RESOURCE = ACTION_IDX; ACTION_IDX += 1

    ATT_IDX = 0
    ATT_IDX_X = ATT_IDX; ATT_IDX += 1
    ATT_IDX_Y = ATT_IDX; ATT_IDX += 1
    ATT_IDX_ORIENTATION = ATT_IDX; ATT_IDX += 1
    ATT_IDX_CAPACITY = ATT_IDX; ATT_IDX += 1
    ATT_IDX_ACCURACY = ATT_IDX; ATT_IDX += 1
    ATT_IDX_COLLECT_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_DEPOSIT_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_ATTACK_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_LINE_OF_SIGHT = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MAX_HEALTH = ATT_IDX; ATT_IDX += 1
    ATT_IDX_HEALTH = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_ROTATION_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_X = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_Y = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_DIRECTION_X = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_DIRECTION_Y = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_MOVE_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_ORIENTATION = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_ROTATION_DIRECTION = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_ROTATION_SPEED = ATT_IDX; ATT_IDX += 1
    ATT_IDX_MOVEMENT_STARTTIME = ATT_IDX; ATT_IDX += 1

    PROJ_ATT_IDX = 0
    PROJECTILE_ATT_IDX_TIME = PROJ_ATT_IDX; PROJ_ATT_IDX += 1
    PROJECTILE_ATT_IDX_SPREAD = PROJ_ATT_IDX; PROJ_ATT_IDX += 1
    PROJECTILE_ATT_IDX_RANGE = PROJ_ATT_IDX; PROJ_ATT_IDX += 1
    PROJECTILE_ATT_IDX_ORIENTATION = PROJ_ATT_IDX; PROJ_ATT_IDX += 1
    PROJECTILE_ATT_IDX_X = PROJ_ATT_IDX; PROJ_ATT_IDX += 1
    PROJECTILE_ATT_IDX_Y = PROJ_ATT_IDX; PROJ_ATT_IDX += 1
    PROJECTILE_ATT_IDX_NUM_PROJECTILES = PROJ_ATT_IDX; PROJ_ATT_IDX += 1


def create_action_vector(action):
    action_vector = np.zeros(StateIndices.ACTION_IDX)
    if action['action-type'] == Actions.IDLE:
        action_vector[StateIndices.ACTION_IS_IDLE] = 1
    elif action['action-type'] == Actions.COLLECT:
        action_vector[StateIndices.ACTION_IS_COLLECT] = 1
        # TODO: set the action resource
    elif action['action-type'] == Actions.DEPOSIT:
        action_vector[StateIndices.ACTION_IS_DEPOSIT] = 1
        # TODO: set the action resource
    elif action['action-type'] == Actions.STRAFE:
        action_vector[StateIndices.ACTION_IS_STRAFE] = 1
        if action['action-args']['is-left']:
            action_vector[StateIndices.ACTION_STRAFE_IS_LEFT] = 1
    elif action['action-type'] == Actions.ATTACK:
        action_vector[StateIndices.ACTION_IS_ATTACK] = 1
    elif action['action-type'] == Actions.MOVE:
        action_vector[StateIndices.ACTION_IS_MOVEMENT] = 1
        action_vector[StateIndices.ACTION_MOVEMENT_DESIRED_ORIENTATION] = \
            action['action-args']['desired-orientation']
        if len(action['action-args']['path']) > 0:
            action_vector[StateIndices.ACTION_MOVEMENT_X] = \
                action['action-args']['path'][0]['x']
            action_vector[StateIndices.ACTION_MOVEMENT_Y] = \
                action['action-args']['path'][0]['y']
    else:
        raise Exception('unknown action type')
    return action_vector


def assign_player_entities(state, assignment, player, player_index, entities_tensor):
    for entity_id, entity in state['entities-by-player'][player].items():
        entity_idx = assignment.get_entity_index(player_index, entity_id)
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_X] = entity['attributes']['x']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_Y] = entity['attributes']['y']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_ORIENTATION] = entity['attributes']['orientation']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_CAPACITY] = entity['attributes']['capacity']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_ACCURACY] = entity['attributes']['accuracy']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_COLLECT_SPEED] = entity['attributes']['collect-speed']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_DEPOSIT_SPEED] = entity['attributes']['deposit-speed']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_ATTACK_SPEED] = entity['attributes']['attack-speed']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_LINE_OF_SIGHT] = entity['attributes']['line-of-sight']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MAX_HEALTH] = entity['attributes']['max-health']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_HEALTH] = entity['attributes']['movement-speed']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_SPEED] = entity['attributes']['rotation-speed']
        entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_ROTATION_SPEED] = entity['attributes']['health']
        if 'movement' in entity:
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_X] = entity['movement']['x']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_Y] = entity['movement']['y']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_DIRECTION_X] = entity['movement']['direction-x']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_DIRECTION_Y] = entity['movement']['direction-y']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_MOVE_SPEED] = entity['movement']['move-speed']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_ORIENTATION] = entity['movement']['orientation']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_ROTATION_DIRECTION] = entity['movement']['rotation-direction']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_ROTATION_SPEED] = entity['movement']['rotation-speed']
            entities_tensor[player_index, entity_idx, StateIndices.ATT_IDX_MOVEMENT_STARTTIME] = entity['movement']['start-time']

        if 'action' in entity and entity['action'] is not None:
            entities_tensor[
                player_index,
                entity_idx,
                StateIndices.ATT_IDX:(StateIndices.ATT_IDX + StateIndices.ACTION_IDX)
            ] = create_action_vector(entity['action'])


def assign_projectiles(state, assignment, projectiles):
    for i, projectile in enumerate(state['projectiles'].values()):
        idx = assignment.get_projectile_index(projectile['id'])
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_TIME] = projectile['time']
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_SPREAD] = projectile['spread']
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_RANGE] = projectile['range']
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_ORIENTATION] = projectile['orientation']
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_X] = projectile['x']
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_Y] = projectile['y']
        projectiles[idx, StateIndices.PROJECTILE_ATT_IDX_NUM_PROJECTILES] = len(projectile['sunk'])


def game_state_to_numpy(state, assignment=None):
    if assignment is None:
        assignment = Assignment(state['player-number'])
    elif assignment.get_player_index(state['player-number']) != 0:
        raise Exception()

    training_dimensions = get_training_dimensions()
    ret = {
        'entities': np.zeros(training_dimensions['state']['entities']),
        'projectiles': np.zeros(training_dimensions['state']['projectiles']),
        'game-time': state['current-time'],
    }

    for player in range(len(state['entities-by-player'])):
        assign_player_entities(
            state,
            assignment,
            player,
            assignment.get_player_index(player),
            ret['entities']
        )

    assign_projectiles(
        state,
        assignment,
        ret['projectiles']
    )
    return ret


def actions_to_numpy(actions, assignment):
    training_dimensions = get_training_dimensions()['actions']
    actions_mat = np.zeros(training_dimensions['actions'])

    for entity_id, action in actions.items():
        entity_idx = assignment.get_entity_index(0, entity_id)
        actions_mat[entity_idx] = create_action_vector(action)

    return {
        'actions': actions_mat
    }


def get_training_dimensions():
    return {
        'state': {
            # type
            'inputs': ['entities', 'projectiles', 'game-time'],
            'entities': (
                NumpyLimits.MAX_NUM_PLAYERS,
                NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER,
                StateIndices.ATT_IDX + StateIndices.ACTION_IDX
            ),
            'projectiles': (
                NumpyLimits.MAX_NUM_PROJECTILES,
                StateIndices.PROJ_ATT_IDX
            ),
            'game-time': [],
            'action_resources': (
                NumpyLimits.MAX_NUM_PLAYERS,
                NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER,
                NumpyLimits.NUM_RESOURCES
            ),
        },
        'actions': {
            'inputs': ['actions'],
            'actions': (
                NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER,
                StateIndices.ACTION_IDX
            ),
        }
    }

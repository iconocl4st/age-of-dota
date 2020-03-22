from datetime import datetime
import json
import os

from .network import Message
from .encoding_limits import Actions
from tf_nueral_network.evaluate_policy import evaluate_policy


def create_game_state(launch_message):
    return {
        'current-time': 0,
        'player-number': launch_message['player-number'],
        'entity-to-player': {},
        'entities-by-player': [
            {}
            for i in range(launch_message['num-players'])
        ],
        'projectiles': {},
    }


def _get_player_entity_object(game_state, id):
    if id not in game_state['entity-to-player']:
        return None
    player = game_state['entity-to-player'][id]
    return game_state['entities-by-player'][player]


def handle_create_entity(application, game_state):
    creation_params = application.client_connection.parse_create_entity()
    player = creation_params['player']
    id = creation_params['id']
    game_state['entity-to-player'][id] = player
    game_state['entities-by-player'][player][id] = creation_params


def handle_delete_entity(application, game_state):
    creation_params = application.client_connection.parse_delete_entity()


def handle_game_time_change(application, game_state):
    new_time = application.client_connection.parse_game_time()
    game_state['current-time'] = new_time['time']


def handle_movement_changed(application, game_state):
    movement_change = application.client_connection.parse_movement_change()
    id = movement_change['entity']
    obj = _get_player_entity_object(game_state, id)
    obj[id]['movement'] = movement_change['movement']


def handle_action_changed(application, game_state):
    changed = application.client_connection.parse_action_changed()
    id = changed['entity-id']
    obj = _get_player_entity_object(game_state, id)
    if obj is None:
        application.logger.log("Entity action changed, but it does not exist, yet.")
        return
    obj[id]['action'] = {
        'action-type': changed['action-type'],
        'action-args': changed['action-args'],
    }
    if changed['action-type'] != Actions.MOVE and changed['action-type'] != Actions.STRAFE:
        if 'movement' in obj[id]:
            del obj[id]['movement']


def handle_projectile_launched(application, game_state):
    p = application.client_connection.parse_projectile_launch()
    game_state['projectiles'][p['id']] = p


def handle_projectile_update(application, game_state):
    update = application.client_connection.parse_projectile_update()
    game_state['projectiles'][update['id']]['sunk'][update['num']] = True


def handle_projectile_destroyed(application, game_state):
    id = application.client_connection.parse_projectile_destroy()
    del game_state['projectiles'][id]


def handle_damage_application(application, game_state):
    damage = application.client_connection.parse_apply_damage()
    id = damage['entity']
    entity_object = _get_player_entity_object(game_state, id)
    entity_object[id]['attributes']['health'] = damage['health']
    if damage['dies']:
        del entity_object[id]
        del game_state['entity-to-player'][id]


def handle_game_over(application, game_state):
    winningPlayer = application.client_connection.parse_close_game()

    with open(application.get_winner_file(), "w+") as outf:
        json.dump({'winner': winningPlayer}, outf)

    application.client_connection.send_close_connection()


def handle_request_actions(application, game_state):
    actions = evaluate_policy(game_state)
    for entityId, action in actions.items():
        application.client_connection.send_action_request(entityId, action)
    with open(application.get_next_game_file(), 'w+') as outfile:
        json.dump(
            { 'state': game_state, 'actions': actions },
            outfile,
            indent=2
        )
    application.client_connection.send_actions_sent()


def handle_close_connection(application):
    application.close_connection = True


def run_game_loop(application, launch_message):
    application.logger.log(json.dumps(launch_message, indent=2))
    game_state = create_game_state(launch_message)

    application.game_directory = os.path.join(
        application.settings['output-directory'],
        "recorded_games",
        (
            'recorded_game_' +
            str(application.player_number) + "_" +
            str(application.lobby_name) + "_" +
            str((datetime.utcnow().timestamp()))
        )
    )
    os.mkdir(application.game_directory)

    while not application.close_connection:
        application.client_connection.parser.read_begin_object(key=None)
        msg_type = application.client_connection.parser.read_number(key="message-type")
        application.logger.log("received message of type: " + Message.get_name(msg_type))

        if msg_type == Message.Types['NOOP']:
            pass
        elif msg_type == Message.Types['CREATE_ENTITY']:
            handle_create_entity(application, game_state)
        elif msg_type == Message.Types['DELETE_ENTITY']:
            handle_delete_entity(application, game_state)
        elif msg_type == Message.Types['GAME_TIME_CHANGE']:
            handle_game_time_change(application, game_state)
        elif msg_type == Message.Types['MOVEMENT_CHANGED']:
            handle_movement_changed(application, game_state)
        elif msg_type == Message.Types['ENTITY_ACTION_CHANGED']:
            handle_action_changed(application, game_state)
        elif msg_type == Message.Types['PROJECTILE_LAUNCHED']:
            handle_projectile_launched(application, game_state)
        elif msg_type == Message.Types['UPDATE_PROJECTILE']:
            handle_projectile_update(application, game_state)
        elif msg_type == Message.Types['PROJECTILE_DESTROYED']:
            handle_projectile_destroyed(application, game_state)
        elif msg_type == Message.Types['ENTITY_RECEIVED_DAMAGE']:
            handle_damage_application(application, game_state)
        elif msg_type == Message.Types['GAME_OVER']:
            handle_game_over(application, game_state)
        elif msg_type == Message.Types['REQUEST_AI_ACTIONS']:
            handle_request_actions(application, game_state)
        elif msg_type == Message.Types['CLOSE_CONNECTION']:
            handle_close_connection(application)
        else:
            application.logger.log("This is not recognized, shutting down.")
            application.close_connection = True
        application.client_connection.parser.read_end_object()

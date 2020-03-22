
from .network import Message


class AiLobbyState:
    REQUESTED_LOBBY_LIST = 0
    JOINED_LOBBY = 1
    SET_READY = 2
    READY_TO_LAUNCH = 3

    def __init__(self):
        self.current_state = AiLobbyState.REQUESTED_LOBBY_LIST


def handle_noop(application, lobby):
    pass


def handle_lobby_list(application, lobby):
    if lobby.current_state != AiLobbyState.REQUESTED_LOBBY_LIST:
        application.logger.log("Illegal state, expected to be in requested lobby")
        application.close_connection = True
        return
    lobbies = application.client_connection.parse_lobbies_list()
    if application.lobby_name not in lobbies:
        application.logger.log("Could not find desired lobby name")
        application.close_connection = True
        return
    application.logger.log("Joining lobby")
    application.client_connection.join_lobby(application.lobby_name, application.player_number)
    lobby.current_state = AiLobbyState.JOINED_LOBBY


def handle_status_update(application, lobby):
    application.logger.log("Received lobby update.")
    status_update = application.client_connection.parse_status_update()
    if (
            not status_update["connected"] or
            status_update["spectating"] or
            status_update["lobby_name"] != application.lobby_name
    ):
        application.logger.log("Bad status update")
        application.close_connection = True
        return

    if lobby.current_state == AiLobbyState.JOINED_LOBBY:
        if status_update["ready"]:
            application.logger.log("Did not expect to be ready yet")
            application.close_connection = True
            return
        application.client_connection.set_ready(True)
        lobby.current_state = AiLobbyState.SET_READY
        application.logger.log("State set to set ready")
    elif lobby.current_state == AiLobbyState.SET_READY:
        if not status_update["ready"]:
            return
        lobby.current_state = AiLobbyState.READY_TO_LAUNCH
        application.logger.log("State set to ready to launch.")
    elif lobby.current_state == AiLobbyState.READY_TO_LAUNCH:
        if not status_update["ready"]:
            application.logger.log("Was ready, but not anymore?!")
            application.close_connection = True
    else:
        application.logger.log("Unexpected state: " + str(current_state))
        application.close_connection = True


def handle_launch(application, lobby):
    if lobby.current_state != AiLobbyState.READY_TO_LAUNCH:
        application.logger.log("Illegal state, expected to be ready to launch")
        application.close_connection = True
        return None

    launch_message = application.client_connection.parse_launched()
    application.client_connection.parser.read_end_object()

    application.logger.log("Launching!!!!")
    return launch_message


def handle_close_connection(application, lobby):
    application.client_connection = True


def run_lobby_loop(application):
    lobby = AiLobbyState()

    while not application.close_connection:
        application.client_connection.parser.read_begin_object(key=None)
        msg_type = application.client_connection.parser.read_number(key="message-type")
        application.logger.log("received message of type: " + Message.get_name(msg_type))
        if msg_type == Message.Types['NOOP']:
            handle_noop(application, lobby)
        elif msg_type == Message.Types['LOBBIES_LIST']:
            handle_lobby_list(application, lobby)
        elif msg_type == Message.Types['LOBBY_STATUS_UPDATE']:
            handle_status_update(application, lobby)
        elif msg_type == Message.Types['CLOSE_CONNECTION']:
            handle_close_connection(application, lobby)
        elif msg_type == Message.Types['LAUNCHED']:
            return handle_launch(application, lobby)
        else:
            application.logger.log("This is not recognized, shutting down.")
            application.close_connection = True
        application.client_connection.parser.read_end_object()
    return None

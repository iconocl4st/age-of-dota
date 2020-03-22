import socket
import json
import os
from datetime import datetime

from .network import create_client_connection
from .network import Message
from .logger import Logger
from .client_application import ClientApplication

from .lobby_loop import run_lobby_loop
from .game_loop import run_game_loop

from tf_nueral_network.policy_function import PolicyHolder


def parse_messages(filename):
    with open(filename, "r") as msgIn:
        msgObj = json.load(msgIn)
        for key, value in msgObj['message-types'].items():
            Message.Types[key] = value


def connect(lobby_name, player_number):
    application = ClientApplication()

    # initialize the network now, before the game begins...
    PolicyHolder.get_policy()

    with open('./settings/client_settings.json', 'r') as settingsFile:
        application.settings = json.load(settingsFile)

    parse_messages(os.path.join(application.settings['output-directory'], "message_types.json"))

    application.lobby_name = lobby_name
    application.player_number = player_number

    now = datetime.now()

    unique_string = lobby_name + "_" + str(player_number)
    with open(os.path.join(application.settings['output-directory'], "ai_log_file_" + unique_string + ".txt"), "w+") as log_file:
        application.logger = Logger(log_file)

        application.logger.log("Started ai at " + str(now))
        application.logger.log("Connecting to lobby '" + lobby_name + "' with player number " + str(player_number))

        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((application.settings['server-address'], application.settings['port']))

        application.logger.log("Connected to server")
        with open(os.path.join(application.settings['output-directory'], "ai_" + unique_string + ".json"), "w+") as json_log_file:
            application.client_connection = create_client_connection(client_socket, json_log_file)

            application.client_connection.send_noop()
            application.client_connection.list_lobbies()

            launch_params = run_lobby_loop(application)
            if launch_params is not None:
                run_game_loop(application, launch_params)
                print('made it out of loop')


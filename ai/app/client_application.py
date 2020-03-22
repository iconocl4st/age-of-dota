
import os

class ClientApplication:
    def __init__(self):
        self.client_connection = None
        self.input_buffer = None
        self.input_buffer_index = 0
        self.lobby_name = None
        self.player_number = None
        self.close_connection = False
        self.settings = None
        self.game_directory = None
        self.game_number = 0

    def get_next_game_file(self):
        self.game_number += 1
        return os.path.join(self.game_directory, str(self.game_number) + '.json')

    def get_winner_file(self):
        return os.path.join(self.game_directory, 'winner.json')

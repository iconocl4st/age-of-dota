
import json
import os


class Record:
    def __init__(self):
        self.state_file = None
        self.winner_num = None

    def __str__(self):
        return "[winner="+str(self.winner_num) + ",file=" + self.state_file + "]"

    def __repr__(self):
        return self.__str__()


def collect_records(recorded_games_dir):
    records = []
    for game_directory in os.listdir(recorded_games_dir):
        if os.path.isfile(game_directory):
            continue
        winner_file = os.path.join(recorded_games_dir, game_directory, "winner.json")
        if not os.path.isfile(winner_file):
            continue
        with open(winner_file, "r") as winner_in:
            winning_player_number = json.load(winner_in)['winner']
        for game_filename in os.listdir(os.path.join(recorded_games_dir, game_directory)):
            game_file = os.path.join(recorded_games_dir, game_directory, game_filename)
            if not os.path.isfile(game_file):
                continue
            if game_filename == 'winner.json':
                continue
            if not game_filename.endswith('.json'):
                continue

            record = Record()
            record.winner_num = winning_player_number
            record.state_file = game_file
            records.append(record)
    return records

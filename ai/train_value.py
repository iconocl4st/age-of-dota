import json
import numpy as np

from app.record_manager import collect_records
from tf_nueral_network.train_value_function import train_value_function
from tf_nueral_network.nn_building_common import get_random_player


def train_value(recorded_games_dir):
    num_states = 10000

    records = collect_records(recorded_games_dir)

    print('training value on', num_states, 'of', len(records))

    games = []
    won = np.zeros(num_states, dtype=bool)

    for game_idx, record_idx in enumerate(np.random.choice(len(records), num_states, replace=True)):
        record = records[record_idx]

        with open(record.state_file, "r") as statein:
            state = json.load(statein)['state']

            games.append(state)
            won[game_idx] = record.winner_num == state['player-number']

    train_value_function(games, won)


if __name__ == "__main__":
    for i in range(5):
        train_value('./output/recorded_games')


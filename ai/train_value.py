import json
import numpy as np

from training_common import collect_records
from tf_nueral_network.train_value_function import train_value_function


def train_value(recorded_games_dir):
    num_states = 10

    records = collect_records(recorded_games_dir)

    print('training on', num_states, 'of', len(records))

    games = []
    player_numbers = np.zeros(num_states, dtype=int)
    won = np.zeros(num_states, dtype=bool)

    for game_idx, record_idx in enumerate(np.random.choice(len(records), num_states, replace=True)):
        record = records[record_idx]

        with open(record.state_file, "r") as statein:
            state = json.load(statein)['state']

            games.append(state)
            player_numbers[game_idx] = np.random.randint(low=0, high=len(state['entities-by-player']))
            won[game_idx] = record.winner_num == player_numbers[game_idx]

    train_value_function(games, player_numbers, won)


if __name__ == "__main__":
    for i in range(10):
        train_value('./output/recorded_games')


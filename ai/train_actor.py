import json
import numpy as np

from app.record_manager import collect_records
from tf_nueral_network.train_actor import train_actor as train_impl
from tf_nueral_network.nn_building_common import get_random_player


def train_actor(recorded_games_dir):
	num_states = 2

	records = collect_records(recorded_games_dir)
	print('training actor on', num_states, 'of', len(records))

	states = []
	actions = []
	for idx in range(num_states):
		record = records[np.random.randint(0, len(records))]

		with open(record.state_file, "r") as state_in:
			j = json.load(state_in)
			states.append(j['state'])
			actions.append(j['actions'])

	train_impl(game_states=states, actions=actions)


if __name__ == "__main__":
	for i in range(1):
		train_actor('./output/recorded_games')


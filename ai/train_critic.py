import json
import numpy as np

from app.record_manager import collect_records
from tf_nueral_network.train_critic import train_simple_critic
from tf_nueral_network.nn_building_common import get_random_player


def train_critic(recorded_games_dir):
	num_states = 10000

	records = collect_records(recorded_games_dir)
	print('training critic on', num_states, 'of', len(records))

	states = []
	actions = []
	next_states = []
	won = []
	for idx in range(num_states):
		record = records[np.random.randint(0, len(records))]

		with open(record.state_file, "r") as state_in:
			j = json.load(state_in)
			states.append(j['state'])
			actions.append(j['actions'])
			player_number = j['state']['player-number']

		if record.next_state is not None:
			with open(record.next_state, "r") as state_in:
				next_state = json.load(state_in)['state']
				if next_state['player-number'] != j['state']['player-number']:
					print(record.state_file)
					raise Exception()
				next_states.append(next_state)
		else:
			next_states.append(None)

		won.append(record.winner_num == player_number)

	train_simple_critic(
		game_states=states,
		actions=actions,
		next_states=next_states,
		won=won
	)


if __name__ == "__main__":
	for i in range(1):
		train_critic('./output/recorded_games')


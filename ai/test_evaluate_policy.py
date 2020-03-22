import json
import numpy as np

from training_common import collect_records
from tf_nueral_network.evaluate_policy import evaluate_policy
from app.assignment import Assignment

def evaluate_random_policy(recorded_games_dir):
    records = collect_records(recorded_games_dir)
    record = records[np.random.randint(low=0, high=len(records))]
    with open(record.state_file, 'r') as statein:
        state = json.load(statein)
    player_number = np.random.randint(low=0, high=len(state['entities-by-player']))
    actions = evaluate_policy(game_state=state, player_number=player_number)
    print(json.dumps(actions, indent=2))

if __name__ == "__main__":
    evaluate_random_policy('/home/thallock/Documents/CLionProjects/playing/output/recorded_games')
import json
import numpy as np

from app.record_manager import collect_records
from tf_nueral_network.evaluate_policy import evaluate_policy

class MockActionsForLoss:


def evaluate_random_policy(recorded_games_dir):
    records = collect_records(recorded_games_dir)
    record = records[np.random.randint(low=0, high=len(records))]
    with open(record.state_file, 'r') as statein:
        state = json.load(statein)['state']
    actions = evaluate_policy(game_state=state)
    print(json.dumps(actions, indent=2))


if __name__ == "__main__":
    evaluate_random_policy('./output/recorded_games')

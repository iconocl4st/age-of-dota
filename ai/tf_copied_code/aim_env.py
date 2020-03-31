import numpy as np
import math
import logging


def _distance_between_angles(alpha, beta):
	# todo, check this
	phi = abs(beta - alpha)
	if phi > math.pi / 2:
		return math.pi - phi
	else:
		return phi


class AimEnv:
	def __init__(self):
		self.max_num_enemies = 3
		self.action_space = [{
			'name': 'which-action',
			'n': 3,
			'type': 'logit'
		}, {
			'name': 'where-to-shoot',
			'n': 3,
			'type': 'numbers'
		}]
		self.state_space = [{
			'name': 'enemy-locations',
			'shape': (3, 2),
		}, {
			'name': 'enemy-exists',
			'shape': (3,),
		}, {
			'name': 'current-location',
			'shape': (2,),
		}, {
			'name': 'health',
			'shape': (1,),
		}]
		self.helpers = [{
			'name': 'angles',
			'shape': (3,)
		}, {
			'name': 'distance',
			'shape': (3,)
		}]
		self.counter = 0
		self.previous_state = self._create_random_state()

	def _create_random_state(self):
		return {
			'enemy-locations': np.random.random((3, 2)),
			'enemy-exists': 1.0 * (np.random.random(3) < 0.75),
			'current-location': np.random.random(2),
			'health': 1.0
		}

	def reset(self):
		next_state, _, _, _ = self.step({
			'which-action': 2,
			'where-to-shoot': np.array([0.0, 0.0, 0.0]),
		})
		self.counter = 0
		return next_state

	def step(self, actions):
		self.counter += 1

		prev_location = self.previous_state['current-location']
		enemy_exists = [e > 0.5 for e in self.previous_state['enemy-exists']]
		enemy_locations = [loc for loc in self.previous_state['enemy-locations']]
		prev_health = self.previous_state['health']

		sup_info = self.calculate_helpers()

		self.previous_state = self._create_random_state()

		num_enemies_shot_at = 0
		num_enemies_missed = 0
		fired = actions['which-action'] == 0
		healed = actions['which-action'] == 1

		unnec = healed and prev_health > 0.95

		for i in range(3):
			if not enemy_exists[i]:
				continue
			required_angle = math.atan2(
				enemy_locations[i][1] - prev_location[1],
				enemy_locations[i][0] - prev_location[0]
			)

			if (
					_distance_between_angles(required_angle, actions['where-to-shoot'][i]) < math.pi / 8
					and fired
			):
				num_enemies_shot_at += 1
			else:
				num_enemies_missed += 1

		next_health = prev_health - 0.05 * num_enemies_missed
		if healed:
			next_health = 1.0
		self.previous_state['health'] = next_health
		reward = 10.0 * num_enemies_shot_at * next_health - 0.5 * num_enemies_missed - (1.0 if unnec else 0.0)
		if next_health < 0.0:
			reward = -100

		logging.debug("location: " + str(prev_location))
		logging.debug("enemy locations: " + str(enemy_locations))
		logging.debug("enemy exists: " + str(enemy_exists))
		logging.debug("health: " + str(prev_health))
		if actions['which-action'] == 0:
			logging.debug("action: " + 'fire')
		elif actions['which-action'] == 1:
			logging.debug("action: " + 'heal')
		else:
			logging.debug("action: " + 'nothing')
		logging.debug("where: " + str(actions['where-to-shoot']))
		logging.debug("number of enemies hit: " + str(num_enemies_shot_at))
		logging.debug("healed unnecessarily: " + str(unnec))
		logging.debug("reward: " + str(reward))
		logging.debug("================================================")

		return (
			self.previous_state,
			reward,
			self.counter > 100 or next_health < 0.0,
			sup_info
		)

	def calculate_helpers(self):
		loc = self.previous_state['current-location']
		return {
			'angles': [
				math.atan2(o[1] - loc[1], o[0] - loc[0])
				for o in self.previous_state['enemy-locations']
			],
			'distance': [
				np.linalg.norm(loc - o)
				for o in self.previous_state['enemy-locations']
			],
		}

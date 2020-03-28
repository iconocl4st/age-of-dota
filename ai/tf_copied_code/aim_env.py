import numpy as np
import math


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
			'shape': (1,),
			'type': 'numbers'
		}]
		self.state_space = [{
			'name': 'enemy-locations',
			'shape': (3, 2),
			'type': 'numbers'
		}, {
			'name': 'enemy-exists',
			'shape': (3,),
			'type': 'numbers'
		}, {
			'name': 'current-location',
			'shape': (2,),
			'type': 'numbers'
		}, {
			'name': 'health',
			'shape': (1,),
			'type': 'numbers'
		}]
		self.counter = 0
		self.previous_state = self._create_random_state()

	def _create_random_state(self):
		enemy_locations = np.random.random((3, 2))
		enemies_exist = 1.0 * (np.random.random(3) < 0.1)
		location = np.random.random(2)
		return [
			enemy_locations,
			enemies_exist,
			location,
			1.0,
		]

	def reset(self):
		next_state, _, _ = self.step([2, 0])
		self.counter = 0
		return next_state

	def step(self, actions):
		self.counter += 1

		pLocation = self.previous_state[2]
		enemy_locations = []
		for i in range(self.max_num_enemies):
			if self.previous_state[1][i] > 0.5:
				enemy_locations.append(self.previous_state[0])
		prev_health = self.previous_state[3]

		self.previous_state = self._create_random_state()

		shot_at_enemy = False
		next_health = prev_health

		if actions[0] == 0:
			requested_angle = actions[1]
			for enemy_location in enemy_locations:
				required_angle = math.atan2(
					enemy_location[1] - pLocation[1],
					enemy_location[0] - pLocation[0],
				)
				if _distance_between_angles(required_angle, requested_angle) < math.pi / 8:
					shot_at_enemy = True
		elif actions[1] == 1:
			if prev_health < 1:
				next_health = 1.0
			else:
				next_health -= 0.01

		next_health -= 0.01 * len(enemy_locations)
		self.previous_state[3] = np.array([next_health])
		return (
			self.previous_state,
			(10.0 if shot_at_enemy else 1) * next_health,
			self.counter > 100,
		)

	def render(self):
		pass

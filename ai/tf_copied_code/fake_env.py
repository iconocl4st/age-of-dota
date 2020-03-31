import numpy as np


class FakeEnv:
	def __init__(self):
		self.action_space = [{
			'name': 'which-action',
			'n': 10,
			'type': 'logit'
		}]
		self.state_space = [{
			'name': 'want',
			'shape': (15,)
		}]
		self.helpers = []
		self.counter = 0
		self.want = 0

	def reset(self):
		next_state, _, _, _ = self.step([0])
		self.counter = 0
		return next_state

	def step(self, actions):
		self.counter += 1
		wanted = self.want
		self.want = np.random.randint(low=0, high=self.action_space[0]['n'])
		s = np.zeros(self.state_space[0]['shape'], dtype=float)
		s[self.want] = 1.0
		return (
			[s],
			(2 + np.random.random() if actions[0] == wanted else 0.0) + np.random.random(),
			self.counter > 100,
			[]
		)

	def render(self):
		pass

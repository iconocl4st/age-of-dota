import numpy as np
import tensorflow as tf


class FakeActionSpace:
	def __init__(self):
		self.n = 10


class FakeObservationSpace:
	def __init__(self):
		self.shape = (15,)


class FakeEnv:
	def __init__(self):
		self.action_space = FakeActionSpace()
		self.observation_space = FakeObservationSpace()
		self.counter = 0
		self.want = 0

	def reset(self):
		next_state, _, _ = self.step(0)
		self.counter = 0
		return next_state

	def step(self, actions):
		self.counter += 1
		wanted = self.want
		self.want = np.random.randint(low=0, high=self.action_space.n)
		s = np.zeros(self.observation_space.shape, dtype=float)
		s[self.want] = 1.0
		return (
			tf.convert_to_tensor(s),
			tf.convert_to_tensor((2 + np.random.random() if actions == wanted else 0.0) + np.random.random()),
			self.counter > 100,
		)

	def render(self):
		pass

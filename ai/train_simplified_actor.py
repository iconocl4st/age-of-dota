
import logging
import numpy as np
import matplotlib.pyplot as plt


from tf_copied_code.simple_architectures import Envs
from tf_copied_code.simple_architectures import Architectures

from tf_copied_code.model_io import load_model
from tf_copied_code.model_io import save_model

from tf_copied_code.advantage_actor_critic import train


logging_is_initialized = False


def initializing_logging():
	global logging_is_initialized

	if logging_is_initialized:
		return
	logging_is_initialized = True

	logging.getLogger().setLevel(logging.DEBUG)
	fh = logging.FileHandler('/home/thallock/Documents/CLionProjects/age-of-dota/output/spam.log')
	fh.setLevel(logging.DEBUG)
	logging.getLogger().addHandler(fh)

	ch = logging.StreamHandler()
	ch.setLevel(logging.INFO)
	logging.getLogger().addHandler(ch)


def main():
	initializing_logging()

	batch_size = 32
	num_updates = 100
	learning_rate = 1e-3

	#env = Envs.fake_env
	env = Envs.aim_env
	# arch = Architectures.fake_arch
	arch = Architectures.aim_arch

	# model = load_model(, learning_rate)
	model = load_model(arch, learning_rate)

	train(model, env, batch_size, num_updates)
	save_model(model)

	plt.style.use('seaborn')
	plt.plot(np.arange(0, len(model.rewards_history), 10), model.rewards_history[::10])
	plt.xlabel('Episode')
	plt.ylabel('Total Reward')
	plt.show()


if __name__ == '__main__':
	for i in range(100):
		main()

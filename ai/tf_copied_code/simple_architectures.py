
from .fake_env import FakeEnv
from .aim_env import AimEnv


class Envs:
	fake_env = FakeEnv()
	aim_env = AimEnv()


class Architectures:
	fake_arch = {
		'name': 'fake_environment',
		'save-path': '/home/thallock/Documents/CLionProjects/age-of-dota/output/saved_networks/fake',
		'actions': Envs.fake_env.action_space,
		'state': Envs.fake_env.state_space,
		'hidden-layers': [{
			'type': 'dense',
			'activation': 'relu',
			'width': 128,
		}]
	}
	aim_arch = {
		'name': 'aim_environment',
		'save-path': '/home/thallock/Documents/CLionProjects/age-of-dota/output/saved_networks/aim',
		'actions': Envs.aim_env.action_space,
		'state': Envs.aim_env.state_space,
		'hidden-layers': [{
			'type': 'dense',
			'activation': 'relu',
			'width': 128,
		}, {
			'type': 'dense',
			'activation': 'relu',
			'width': 128,
		}]
	}

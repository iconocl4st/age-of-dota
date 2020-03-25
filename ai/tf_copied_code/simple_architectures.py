
from .fake_env import FakeEnv


class Envs:
	fake_env = FakeEnv()


def _compile(arch):
	return {
		**arch,
		'layers-by-name': {layer['name']: layer for layer in arch['layers']}
	}


class Architectures:
	fake_arch = _compile({
		'name': 'face_environment',
		'save-path': '/home/thallock/Documents/CLionProjects/age-of-dota/output/saved_networks/fake',
		'layers': [{
			'name': 'hidden1',
			'type': 'dense',
			'activation': 'relu',
			'width': 128,
		}, {
			'name': 'hidden2',
			'type': 'dense',
			'activation': 'relu',
			'width': 128,
		}, {
			'name': 'value',
			'type': 'dense',
			'activation': None,
			'width': 1,
		}, {
			'name': 'policy_logits',
			'type': 'dense',
			'width': 10,
			'activation': None
		}]
	})

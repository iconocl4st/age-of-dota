import os
import tensorflow as tf
import hashlib
import json
import numpy as np
import logging

from .advantage_actor_critic import logits_loss
from .advantage_actor_critic import value_loss


def _create_layer(layer_obj):
	if layer_obj['type'] == 'dense':
		return tf.keras.layers.Dense(
			layer_obj['width'],
			activation=layer_obj['activation'],
			name=layer_obj['name']
		)


def _hash_model(model_arch):
	m = hashlib.md5()
	for o in model_arch['actions']:
		m.update(json.dumps(o, indent=2).encode(encoding='UTF-8'))
	m.update("::::".encode(encoding='UTF-8'))
	for o in model_arch['state']:
		m.update(json.dumps(o, indent=2).encode(encoding='UTF-8'))
	m.update("::::".encode(encoding='UTF-8'))
	for o in model_arch['hidden-layers']:
		m.update(json.dumps(o, indent=2).encode(encoding='UTF-8'))
	m.update("::::".encode(encoding='UTF-8'))
	return m.hexdigest()


def _compile(model, learning_rate, actions):
	model.compile(
		optimizer=tf.keras.optimizers.RMSprop(lr=learning_rate),
		# Define separate losses for policy logits and value estimate.
		loss=[value_loss, value_loss] + [
			logits_loss if action['type'] == 'logit' else value_loss
			for action in actions
		]
	)
	return model


def _create_a_hidden(model_arch, name, last_shape):
	ret = []
	for idx, desc in enumerate(model_arch['hidden-layers']):
		ret.append(_create_layer({
			**desc,
			'name': 'hidden_' + name + '_' + str(idx)
		}))

	ret.append(tf.keras.layers.Dense(np.prod(last_shape), name='last_dense_' + name))
	ret.append(tf.keras.layers.Reshape(last_shape, name='output_' + name))
	return ret


def _eval_tensors(tensors, x):
	for layer in tensors:
		x = layer(x)
	return x


class Model(tf.keras.Model):
	def __init__(self, networks, actions, states):
		super().__init__('mlp_policy')
		self.networks = networks
		self.actions = actions
		self.states = states

	def call(self, inputs, **kwargs):
		x = tf.convert_to_tensor(inputs)
		return [
			_eval_tensors(self.networks['value'], x),
			_eval_tensors(self.networks['return'], x),
		] + [
			_eval_tensors(action_out, x)
			for action_out in self.networks['actions']
		]


def _create_empty_model(model_arch, learning_rate):
	model = Model(
		{
			'value': _create_a_hidden(model_arch, 'value', (1,)),
			'return': _create_a_hidden(model_arch, 'return', (1,)),
			'actions': [
				_create_a_hidden(
					model_arch,
					action['name'],
					(action['n'],) if action['type'] == 'logit' else action['shape']
				)
				for action in model_arch['actions']
			],
		},
		model_arch['actions'],
		model_arch['state']
	)

	model.rewards_history = []

	path = model_arch['save-path']
	if not os.path.exists(path):
		os.mkdir(path)

	tf.keras.utils.plot_model(
		model,
		to_file=os.path.join(path, 'model.png'),
		rankdir='LR',
		show_shapes=True,
		show_layer_names=True,
		expand_nested=True
	)

	return _compile(model, learning_rate, model_arch['actions'])


def save_model(model_arch, model):
	path = model_arch['save-path']
	if not os.path.exists(path):
		os.mkdir(path)
	with open(os.path.join(path, 'architecture.json'), "w") as ma:
		json.dump(model_arch, ma, indent=2)
	with open(os.path.join(path, 'rewards.json'), 'w') as rewards_out:
		json.dump(model.rewards_history, rewards_out, indent=2)
	model.save(path)
	logging.info('successfully saved model at ' + path)


def load_model(model_arch, learning_rate):
	if 'save-path' not in model_arch:
		logging.error('no path given')
		raise Exception()

	path = model_arch['save-path']
	if not os.path.exists(path):
		logging.info('no existing model found at ' + path)
		return _create_empty_model(model_arch, learning_rate)
	prev_arch_path = os.path.join(path, 'architecture.json')
	if not os.path.exists(prev_arch_path):
		logging.info('existing model did not contain an architecture at ' + path)
		return _create_empty_model(model_arch, learning_rate)

	with open(prev_arch_path) as ina:
		prev_arch = json.load(ina)
		if _hash_model(model_arch) != _hash_model(prev_arch):
			logging.info('detected change in model at ' + path)
			return _create_empty_model(model_arch, learning_rate)

	try:
		model = tf.keras.models.load_model(path, compile=False)
	except:
		logging.info('exception while loading model ' + path)
		return _create_empty_model(model_arch, learning_rate)

	rewards_path = os.path.join(path, 'rewards.json')
	if os.path.exists(rewards_path):
		with open(rewards_path, 'r') as rewards_in:
			model.rewards_history = json.load(rewards_in)
	else:
		model.rewards_history = []

	model.actions = model_arch['actions']
	model.states = model_arch['state']

	logging.info('successfully loaded model at ' + path)

	return _compile(model, learning_rate, model_arch['actions'])

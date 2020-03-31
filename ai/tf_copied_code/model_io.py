import os
import tensorflow as tf
import hashlib
import json
import numpy as np
import logging
import datetime

from .advantage_actor_critic import logits_loss
from .advantage_actor_critic import value_loss
from .advantage_actor_critic import action_loss
from .advantage_actor_critic import supplementary_loss


class ModelWrapper:
	def __init__(self):
		self.model = None
		self.arch = None
		self.rewards_history = None
		self.tb_writer = None


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
	for o in model_arch['supplementary-info']:
		m.update(json.dumps(o, indent=2).encode(encoding='UTF-8'))
	return m.hexdigest()


def _compile(wrapper, learning_rate):
	log_dir = '/home/thallock/Documents/CLionProjects/age-of-dota/output/tensorboard/'
	log_dir = os.path.join(log_dir, datetime.datetime.now().strftime("%Y%m%d-%H%M%S"))
	tensorboard_callback = tf.keras.callbacks.TensorBoard(log_dir=log_dir)
	tensorboard_callback.set_model(wrapper.model)

	values_file = os.path.join(log_dir, 'values')
	wrapper.tb_writer = tf.summary.create_file_writer(values_file)

	wrapper.model.compile(
		optimizer=tf.keras.optimizers.RMSprop(lr=learning_rate),
		loss=[value_loss, value_loss] + [
			logits_loss if action['type'] == 'logit' else action_loss
			for action in wrapper.arch['actions']
		] + [
			supplementary_loss
			for _ in wrapper.arch['supplementary-info']
		]
	)


def _create_empty_model(model_arch, learning_rate):
	input_layers = {}
	flattened = []
	for state_entry in model_arch['state']:
		input_layer = tf.keras.Input(shape=state_entry['shape'], name='input-' + state_entry['name'])
		input_layers[state_entry['name']] = input_layer
		flattened.append(tf.keras.layers.Flatten(name='flatten-' + state_entry['name'])(input_layer))

	x = tf.keras.layers.Concatenate(name='concat-inputs')(flattened)

	for idx, desc in enumerate(model_arch['hidden-layers']):
		if desc['type'] == 'dense':
			x = tf.keras.layers.Dense(
				desc['width'],
				activation=desc['activation'],
				name='hidden-' + str(idx) + '-dense'
			)(x)
			x = tf.keras.layers.Dropout(rate=0.2, name='hidden-' + str(idx) + '-dropout')(x)
		else:
			raise Exception()

	outputs = [
		tf.keras.layers.Dense(1, activation='relu', name='value')(x),
		tf.keras.layers.Dense(1, activation='relu', name='return')(x),
	]

	for action in model_arch['actions']:
		if action['type'] == 'logit':
			shape = (action['n'], )
		else:
			shape = (action['n'], 2,)
		action_out = tf.keras.layers.Dense(np.prod(shape), activation='relu', name='dense-' + action['name'])(x)
		action_out = tf.keras.layers.Reshape(shape, name='output-' + action['name'])(action_out)
		outputs.append(action_out)

	for sup in model_arch['supplementary-info']:
		sup_out = tf.keras.layers.Dense(np.prod(sup['shape']), activation='linear', name='dense-sup-' + sup['name'])(x)
		sup_out = tf.keras.layers.Reshape(sup['shape'], name='output-' + sup['name'])(sup_out)
		outputs.append(sup_out)

	wrapper = ModelWrapper()
	wrapper.model = tf.keras.Model(
		inputs=input_layers,
		outputs=outputs,
		name=model_arch['name']
	)
	wrapper.rewards_history = []
	wrapper.arch = model_arch

	path = model_arch['save-path']
	if not os.path.exists(path):
		os.mkdir(path)

	tf.keras.utils.plot_model(
		wrapper.model,
		to_file=os.path.join(path, 'model.png'),
		rankdir='LR',
		show_shapes=True,
		show_layer_names=True,
		expand_nested=True
	)
	_compile(wrapper, learning_rate)
	return wrapper


def save_model(wrapper):
	path = wrapper.arch['save-path']
	if not os.path.exists(path):
		os.mkdir(path)
	with open(os.path.join(path, 'architecture.json'), "w") as ma:
		json.dump(wrapper.arch, ma, indent=2)
	with open(os.path.join(path, 'rewards.json'), 'w') as rewards_out:
		json.dump(wrapper.rewards_history, rewards_out, indent=2)
	wrapper.model.save(path)
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

	wrapper = ModelWrapper()
	wrapper.arch = model_arch
	try:
		wrapper.model = model = tf.keras.models.load_model(path, compile=False)
	except:
		logging.info('exception while loading model ' + path)
		return _create_empty_model(model_arch, learning_rate)

	rewards_path = os.path.join(path, 'rewards.json')
	if os.path.exists(rewards_path):
		with open(rewards_path, 'r') as rewards_in:
			wrapper.rewards_history = json.load(rewards_in)
	else:
		wrapper.rewards_history = []

	logging.info('successfully loaded model at ' + path)
	_compile(wrapper, learning_rate)

	return wrapper

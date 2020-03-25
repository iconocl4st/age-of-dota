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
	for layer in model_arch['layers']:
		m.update(json.dumps(layer).encode(encoding='UTF-8'))
	return m.hexdigest()


def _compile(model, learning_rate):
	model.compile(
		optimizer=tf.keras.optimizers.RMSprop(lr=learning_rate),
		# Define separate losses for policy logits and value estimate.
		loss=[logits_loss, value_loss]
	)
	return model


def _create_empty_model(model_arch, learning_rate):
	ret = {}
	for layer_obj in model_arch['layers']:
		layer = _create_layer(layer_obj)
		ret[layer_obj['name']] = layer

	model = Model(ret)

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

	return _compile(model, learning_rate)


# TODO:
class ProbabilityDistribution(tf.keras.Model):
	def call(self, logits, **kwargs):
		# Sample a random categorical action from the given logits.
		return tf.squeeze(tf.random.categorical(logits, 1), axis=-1)


class Model(tf.keras.Model):
	def __init__(self, layer_map):
		super().__init__('mlp_policy')

		self.hidden1 = layer_map['hidden1']
		self.hidden2 = layer_map['hidden2']
		self.value = layer_map['value']
		# Logits are unnormalized log probabilities.
		self.logits = layer_map['policy_logits']
		self.dist = ProbabilityDistribution()

	def call(self, inputs, **kwargs):
		# Inputs is a numpy array, convert to a tensor.
		# Separate hidden layers from the same input tensor.
		hidden_logs = self.hidden1(inputs)
		hidden_vals = self.hidden2(inputs)
		return self.logits(hidden_logs), self.value(hidden_vals)


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

	logging.info('successfully loaded model at ' + path)

	return _compile(model, learning_rate)

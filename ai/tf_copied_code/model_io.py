import os
import tensorflow as tf
import hashlib
import json
import numpy as np
import logging


def create_layer(layer_obj):
	if layer_obj['type'] == 'dense':
		return tf.keras.layers.Dense(
			layer_obj['width'],
			activation=layer_obj['activation'],
			name=layer_obj['name']
		)


def create_empty_model(model_arch):
	ret = {}
	for layer_obj in model_arch['layers']:
		layer = create_layer(layer_obj)
		ret[layer_obj['name']] = layer
	return ret


def hash_model(model_arch):
	m = hashlib.md5()
	for layer in model_arch['layers']:
		m.update(json.dumps(layer).encode(encoding='UTF-8'))
	return m.hexdigest()


def save_model(model_arch, layers):
	path = model_arch['save-path']
	if not os.path.exists(path):
		os.mkdir(path)
	with open(os.path.join(path, 'architecture.json'), "w") as ma:
		json.dump(model_arch, ma, indent=2)
	for layer_name, layer in layers.items():
		weights = layer.get_weights()
		weights_path = os.path.join(path, layer_name + '.weights')
		np.save(weights_path, weights, allow_pickle=True)

		config = layer.get_config()
		weights_path = os.path.join(path, layer_name + '.config.json')
		with open(weights_path, "w") as configout:
			json.dump(config, configout, indent=2)
	logging.info('successfully saved model at ' + path)


def load_model(model_arch):
	if 'save-path' not in model_arch:
		logging.info('no path given')
		return create_empty_model(model_arch)

	path = model_arch['save-path']
	if not os.path.exists(path):
		logging.info('no existing model found at ' + path)
		return create_empty_model(model_arch)
	prev_arch_path = os.path.join(path, 'architecture.json')
	if not os.path.exists(prev_arch_path):
		logging.info('existing model did not contain an architecture at ' + path)
		return create_empty_model(model_arch)

	with open(prev_arch_path) as ina:
		prev_arch = json.load(ina)
		if hash_model(model_arch) != hash_model(prev_arch):
			logging.info('detected change in model at ' + path)
			return create_empty_model(model_arch)

	model = {}
	for layer in model_arch['layers']:
		layer_name = layer['name']
		weights_path = os.path.join(path, layer_name + '.weights.npy')
		if not os.path.exists(weights_path):
			continue
		weights = np.load(weights_path, allow_pickle=True)

		config_path = os.path.join(path, layer_name + '.config.json')
		if not os.path.exists(config_path):
			model[layer_name] = create_layer(layer)
		else:
			with open(config_path, "r") as configin:
				config = json.load(configin)
			if layer['type'] == 'dense':
				model[layer_name] = tf.keras.layers.Dense.from_config(config)

		print('wait')
	logging.info('successfully loaded model at ' + path)
	return model


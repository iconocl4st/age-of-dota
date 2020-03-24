from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf
import numpy as np


class Sizes:
	STATE_DIM = (10, 10)
	ACTION_INPUT_DIM = (10, 15)
	ACTION_OUTPUT_DIM = 20


def get_random_data():
	len = np.random.randint(low=10, high=30)
	return (
		np.random.normal(size=tuple([len] + [d for d in Sizes.STATE_DIM])),
		np.random.normal(size=tuple([len] + [d for d in Sizes.ACTION_INPUT_DIM])),
		np.random.normal(size=(len, Sizes.ACTION_OUTPUT_DIM)),
	)


def run_simplified_network():
	actor_state_input = tf.keras.Input(shape=Sizes.STATE_DIM, dtype=tf.float32)
	actor_action_input = tf.keras.Input(shape=Sizes.ACTION_INPUT_DIM, dtype=tf.float32)

	actor_flatten = tf.keras.layers.Flatten()(actor_state_input)
	actor_dense_1 = tf.keras.layers.Dense(5, activation='relu')(actor_flatten)
	actor_dense_2 = tf.keras.layers.Dense(5, activation='relu')(actor_dense_1)
	actor_output = tf.keras.layers.Dense(Sizes.ACTION_OUTPUT_DIM, activation='relu')(actor_dense_2)

	actor = tf.keras.Model(inputs=[actor_state_input, actor_action_input], outputs=actor_output, name='test_actor')

	critic_state_input = tf.keras.Input(shape=Sizes.STATE_DIM, dtype=tf.float32)
	critic_action_input = tf.keras.Input(shape=Sizes.ACTION_INPUT_DIM, dtype=tf.float32)

	critic_state_flatten = tf.keras.layers.Flatten()(critic_state_input)
	critic_action_flatten = tf.keras.layers.Flatten()(critic_action_input)
	critic_concatenate = tf.keras.layers.Concatenate()([critic_state_flatten, critic_action_flatten])
	critic_dense_1 = tf.keras.layers.Dense(5, activation='relu')(critic_concatenate)
	critic_dense_2 = tf.keras.layers.Dense(5, activation='relu')(critic_dense_1)
	critic_dense_3 = tf.keras.layers.Dense(5, activation='relu')(critic_dense_2)
	critic_output = tf.keras.layers.Dense(1, activation='sigmoid')(critic_dense_3)

	critic = tf.keras.Model(inputs=[critic_state_input, critic_action_input], outputs=critic_output, name='test_actor')

	actor.compile(optimizer='adam', loss='mse')
	state_data, action_data, action_output = get_random_data()
	actor.fit([state_data, action_data], action_output, epochs=2)


if __name__ == "__main__":
	run_simplified_network()
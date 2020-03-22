import tensorflow as tf


class MemorySetter:
	memory_set = False

	@classmethod
	def set_memory_usage(cls):
		if cls.memory_set:
			return
		cls.memory_set = True
		physical_devices = tf.config.list_physical_devices('GPU')
		tf.config.experimental.set_memory_growth(physical_devices[0], True)

		# tf.config.gpu.set_per_process_memory_fraction(0.75)
		# tf.config.gpu.set_per_process_memory_growth(True)



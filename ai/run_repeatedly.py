
import numpy as np
import subprocess
import time


def run_infinitely():
	while True:
		process = subprocess.Popen(
			"/home/thallock/Documents/CLionProjects/age-of-dota/cmake-build-debug/both",
			stdout=subprocess.PIPE,
			stderr=subprocess.PIPE,
			shell=True
		)
		time.sleep(60)
		try:
			process.wait(120)
		except subprocess.TimeoutExpired:
			print('killing')
			process.kill()


if __name__ == "__main__":
	run_infinitely()


//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_RANDOMGENERATOR_H
#define PLAYING_RANDOMGENERATOR_H

#include <random>
#include <stdexcept>

class TruncatedGaussian {
public:
	double mean;
	double stddev;
	double minimum;
	double maximum;

	TruncatedGaussian(double mean, double stddev, double minimum, double maximum) :
		mean{mean},
		stddev{stddev},
		minimum{minimum},
		maximum{maximum} {}

	TruncatedGaussian(double fixedValue) : TruncatedGaussian{fixedValue, 1.0, fixedValue, fixedValue} {}
	TruncatedGaussian() : TruncatedGaussian{0.0} {}
};


class RandomGenerator {
private:
	std::default_random_engine generator;
	std::uniform_real_distribution<double> doubleDistribution;
	std::uniform_int_distribution<int> intDistribution;
	std::normal_distribution<double> gaussianDistribution;

public:
	RandomGenerator(unsigned int seed) :
		generator{seed},
		doubleDistribution{0, 1},
		intDistribution{0, INT32_MAX},
		gaussianDistribution{0, 1} {}

	int nextInt(int min, int max) {
		return min + (intDistribution(generator) % (max - min));
	}
	int nextInt() {
		return nextInt(0, INT32_MAX);
	}

	double nextDouble(double min, double max) {
		return min + (doubleDistribution(generator) * (max - min));
	}

	double nextDouble() {
		return doubleDistribution(generator);
	}

	double nextGaussian(const TruncatedGaussian& gaussian)  {
		double ret = gaussian.mean + doubleDistribution(generator) * gaussian.stddev;
		if (ret < gaussian.minimum) return gaussian.minimum;
		if (ret > gaussian.maximum) return gaussian.maximum;
		return ret;
	}

	bool nextBool() {
		return nextInt(0, 2);
//		if (r == 0) {
//			return false;
//		} else if (r == 1) {
//			return true;
//		} else {
//			// TODO
//			throw std::runtime_error("Idk what that method does");
//		}
	}

	std::string nextString(int length);

	double nextAngle();
};


#endif //PLAYING_RANDOMGENERATOR_H

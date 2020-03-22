
//
// Created by thallock on 3/11/20.
//

#include "RandomGenerator.h"
#include "angles.h"

#include <cmath>
#include <sstream>

std::string possible_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

std::string RandomGenerator::nextString(int length) {
	std::ostringstream ss;
	for (int i = 0; i < length; i++) {
		ss << possible_characters.at(nextInt(0, possible_characters.size()));
	}
	return ss.str();
}

double RandomGenerator::nextAngle() {
	return normalize_angle(nextDouble(-M_PI, M_PI));
}

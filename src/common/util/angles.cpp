//
// Created by thallock on 3/12/20.
//

#include "angles.h"

#include <cmath>

double normalize_angle(double theta) {
	while (theta > +M_PI) theta -= 2 * M_PI;
	while (theta < -M_PI) theta += 2 * M_PI;
	return theta;
}

double getDistanceBetweenAngles(double a1, double a2) {
	double a = std::abs(normalize_angle(a2) - normalize_angle(a1));
	if (a <= M_PI) {
		return a;
	}
	return 2 * M_PI - a;
}

double getAngleDirection(double from, double to) {
	double a = normalize_angle(to + 2 * M_PI - from);
	if (a >= 0) {
		return 1.0;
	} else {
		return -1.0;
	}
}
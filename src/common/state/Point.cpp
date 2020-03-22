//
// Created by thallock on 3/11/20.
//

#include <cmath>
#include "Point.h"

double Point::norm() {
	return std::sqrt(x * x + y * y);
}

void Point::placeWithinBounds(double width, double height) {
	x = std::max(0.0, std::min(width, x));
	y = std::max(0.0, std::min(height, y));
}

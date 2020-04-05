//
// Created by thallock on 4/5/20.
//

#include "branch_direction.h"

std::array<branch_direction, 4> all_directions{{
	UPPER_RIGHT,
	UPPER_LEFT,
	LOWER_RIGHT,
	LOWER_LEFT,
}};

std::array<branch_direction, 4>& getAllDirections() {
	return all_directions;
}


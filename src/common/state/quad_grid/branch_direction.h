//
// Created by thallock on 4/5/20.
//

#ifndef AGE_OF_DOTA_BRANCHDIRECTION_H
#define AGE_OF_DOTA_BRANCHDIRECTION_H

#include <array>

enum branch_direction {
	NONE,
	UPPER_LEFT,
	UPPER_RIGHT,
	LOWER_LEFT,
	LOWER_RIGHT,
};

std::array<branch_direction, 4>& getAllDirections();


#endif //AGE_OF_DOTA_BRANCHDIRECTION_H

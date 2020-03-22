//
// Created by thallock on 3/12/20.
//

#include <iostream>
#include <cmath>

#include "../../common/util/angles.h"

void f() {
	std::cout << normalize_angle(M_PI) << std::endl;
	std::cout << normalize_angle(2 * M_PI) << std::endl;
	std::cout << normalize_angle(M_PI / 2) << std::endl;
	std::cout << normalize_angle(-M_PI / 2) << std::endl;
	std::cout << "=============================================" << std::endl;
	std::cout << getDistanceBetweenAngles(M_PI, M_PI) << std::endl;
	std::cout << getDistanceBetweenAngles(M_PI, M_PI / 2) << std::endl;
	std::cout << getDistanceBetweenAngles(M_PI / 2, M_PI) << std::endl;
	std::cout << getDistanceBetweenAngles(M_PI / 2, -M_PI / 2) << std::endl;
	std::cout << getDistanceBetweenAngles(-M_PI / 2, M_PI / 2) << std::endl;
	std::cout << getDistanceBetweenAngles(3 * M_PI / 4, -M_PI / 2) << std::endl;
	std::cout << getDistanceBetweenAngles(-M_PI / 2, 3 * M_PI / 4) << std::endl;
	std::cout << getDistanceBetweenAngles(3 * M_PI / 4, -3 * M_PI / 4) << std::endl;
	std::cout << getDistanceBetweenAngles(-3 * M_PI / 4, 3 * M_PI / 4) << std::endl;
	std::cout << "=============================================" << std::endl;
	std::cout << getAngleDirection(0, M_PI / 2) << std::endl;
	std::cout << getAngleDirection(0, -M_PI / 2) << std::endl;
	std::cout << getAngleDirection(3 * M_PI / 4, -3 * M_PI / 4) << std::endl;
	std::cout << "=============================================" << std::endl;
}
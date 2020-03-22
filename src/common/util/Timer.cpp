//
// Created by thallock on 3/11/20.
//

#include "Timer.h"

#include <thread>


typedef std::chrono::high_resolution_clock::time_point time_point;

void Timer::waitForNextTick() {
	time_point now = std::chrono::high_resolution_clock::now();

	time_point desired = desiredTickStart + fixedRateMs;
	time_point minimum = now + minimumWait;

	if (desired > minimum) {
		desiredTickStart = desired;
	} else {
		desiredTickStart = minimum;
	}
	std::this_thread::sleep_for(desiredTickStart - now);
}

Timer::Timer(std::chrono::milliseconds fixedRateMs) :
	minimumWait{1},
	fixedRateMs{fixedRateMs},
	desiredTickStart{std::chrono::high_resolution_clock::now()} {}

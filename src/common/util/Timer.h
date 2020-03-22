//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_TIMER_H
#define PLAYING_TIMER_H


#include <chrono>

class Timer {
private:
	std::chrono::milliseconds minimumWait;
	std::chrono::milliseconds fixedRateMs;
	std::chrono::high_resolution_clock::time_point desiredTickStart;
public:
	Timer(std::chrono::milliseconds fixedRateMs);

	void waitForNextTick();
};


#endif //PLAYING_TIMER_H

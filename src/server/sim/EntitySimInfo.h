//
// Created by thallock on 3/12/20.
//

#ifndef PLAYING_ENTITYSIMINFO_H
#define PLAYING_ENTITYSIMINFO_H

#include "ServerGameState.h"
#include "TickInfo.h"
#include "StateChange.h"

class EntitySimInfo {
public:
	EntityId entity;
	ServerGameState *gState;
	TickInfo &tickInfo;
	StateChange &delta;
	void *context;

	EntitySimInfo(
		ServerGameState *gState,
		TickInfo &tickInfo,
		StateChange &delta
	) :
		entity{0},
		gState{gState},
		tickInfo{tickInfo},
		delta{delta} {}
};


#endif //PLAYING_ENTITYSIMINFO_H

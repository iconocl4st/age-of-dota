//
// Created by thallock on 3/12/20.
//

#ifndef PLAYING_SCRIPTAI_H
#define PLAYING_SCRIPTAI_H

#include <chrono>
#include "RandomMovementAiGroup.h"

class ScriptAi {
public:
	std::chrono::milliseconds duration;

	RandomMovementAiGroup movementAiGroup;
public:

	void addEntity(ScriptClientContext& cntxt, EntityId entityId);
	AiGroup& getRandomAiGroup();
};


#endif //PLAYING_SCRIPTAI_H

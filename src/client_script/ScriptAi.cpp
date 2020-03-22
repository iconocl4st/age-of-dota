//
// Created by thallock on 3/12/20.
//

#include "ScriptAi.h"


AiGroup& ScriptAi::getRandomAiGroup() {
	return movementAiGroup;
}

void ScriptAi::addEntity(ScriptClientContext& cntxt, EntityId entityId) {
	getRandomAiGroup().addEntity(cntxt, entityId);
}

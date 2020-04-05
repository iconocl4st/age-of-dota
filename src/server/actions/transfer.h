//
// Created by thallock on 4/2/20.
//

#ifndef AGE_OF_DOTA_TRANSFER_H
#define AGE_OF_DOTA_TRANSFER_H


#include <src/common/state/definitions.h>
#include <src/server/sim/ServerGameState.h>
#include <src/server/sim/EntitySimInfo.h>

class TransferRequest {
	ResourceId resourceType;
	Entity& from;
	Entity& to;

	ServerGameState *gState;
	TickInfo &tickInfo;
	StateChange &delta;
};

EntityId getInteractionEntity(Entity& reader, ServerGameState *gState);

void transfer(TransferRequest& request);

#endif //AGE_OF_DOTA_TRANSFER_H

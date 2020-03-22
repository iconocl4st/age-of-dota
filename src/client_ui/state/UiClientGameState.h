//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_UICLIENTGAMESTATE_H
#define PLAYING_UICLIENTGAMESTATE_H


#include "../../common/state/CommonState.h"
#include "../../client_common/state/ClientGameState.h"

#include "../gl/GraphicsModel.h"

class UiClientGameState {
public:
	ClientGameState clientGameState;

	std::map<EntityId, GraphicsModel*> models;

	UiClientGameState(PlayerLaunchOptions& options) :
		clientGameState{options} {}
};


#endif //PLAYING_UICLIENTGAMESTATE_H

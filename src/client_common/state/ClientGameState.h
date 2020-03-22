//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_CLIENTGAMESTATE_H
#define PLAYING_CLIENTGAMESTATE_H


#include "../../common/state/CommonState.h"
#include "../../common/network/PlayerLaunchOptions.h"
#include "../../common/state/LocationManager.h"

class ClientGameState {
public:
    int playerNumber;
	aod::common::state::CommonState sharedState;

    ClientGameState(PlayerLaunchOptions& options);
};


#endif //PLAYING_CLIENTGAMESTATE_H

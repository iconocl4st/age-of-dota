//
// Created by thallock on 3/9/20.
//

#include "ClientGameState.h"

ClientGameState::ClientGameState(PlayerLaunchOptions &options) :
    playerNumber{options.playerNumber},
    sharedState(options.spec) {

}

//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_SCRIPTCLIENTCONTEXT_H
#define PLAYING_SCRIPTCLIENTCONTEXT_H


#include "../common/network/ConnectionContext.h"
#include "../common/util/RandomGenerator.h"
#include "ScriptAi.h"
#include "../common/util/logging_levels.h"
#include "../client_common/state/ClientGameState.h"

class ScriptClientContext {
public:
	RandomGenerator random;
	ConnectionContext& connectionContext;
	ClientGameState clientState;

	Logger logger;

	ScriptAi scriptAi;

	ScriptClientContext(unsigned int seed, ConnectionContext&, PlayerLaunchOptions& options);
};


#endif //PLAYING_SCRIPTCLIENTCONTEXT_H

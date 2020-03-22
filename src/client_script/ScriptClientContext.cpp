//
// Created by thallock on 3/11/20.
//

#include "ScriptClientContext.h"


ScriptClientContext::ScriptClientContext(
	unsigned int seed,
	ConnectionContext& connectionContext,
	PlayerLaunchOptions& options
) :
	random{seed},
	connectionContext{connectionContext},
	clientState{options} {}



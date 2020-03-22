//
// Created by thallock on 3/8/20.
//

#ifndef PLAYING_SERVERGAMEHANDLER_H
#define PLAYING_SERVERGAMEHANDLER_H

#include "../../common/network/MessageHandler.h"
#include "../app/ServerContext.h"

class ServerGameHandler : public MessageHandler {
private:
	aod::server::app::ServerContext& serverContext;

public:
	ServerGameHandler(aod::server::app::ServerContext& serverContext) :
		serverContext{serverContext} {}

    void handleMessage(MessageHandlingArgs& context);

    void debug(JsonWriter& writer);
};


#endif //PLAYING_SERVERGAMEHANDLER_H

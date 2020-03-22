//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_SERVERMESSAGEHANDLER_H
#define PLAYING_SERVERMESSAGEHANDLER_H

#include "../../common/network/MessageHandler.h"
#include "../app/ServerContext.h"


class ServerMessageHandler : public MessageHandler {
private:
	aod::server::app::ServerContext& serverContext;
public:
	ServerMessageHandler(aod::server::app::ServerContext& sc) : serverContext{sc} {}

    void handleMessage(MessageHandlingArgs& context);

    void debug(JsonWriter& writer);
};

#endif //PLAYING_SERVERMESSAGEHANDLER_H

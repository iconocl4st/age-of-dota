//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_LOBBYHANDLER_H
#define PLAYING_LOBBYHANDLER_H


#include <utility>

#include "../../common/network/MessageHandler.h"
#include "../app/ServerContext.h"

class LobbyHandler : public MessageHandler {
private:
	aod::server::app::ServerContext& serverContext;

public:
    LobbyHandler(aod::server::app::ServerContext& sc) : serverContext{sc} {}

    void handleMessage(MessageHandlingArgs& context);

    void debug(JsonWriter& writer);
};




#endif //PLAYING_LOBBYHANDLER_H

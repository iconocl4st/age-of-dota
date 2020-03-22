//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_CLIENTMESSAGEHANDLER_H
#define PLAYING_CLIENTMESSAGEHANDLER_H


#include "../../common/network/MessageHandler.h"

namespace aod::client::app {

class UiClientContext;

class ClientMessageHandler : public MessageHandler {
private:
	UiClientContext& clientContext;

public:
	ClientMessageHandler(UiClientContext& context);

	void handleMessage(MessageHandlingArgs& context);

	void debug(JsonWriter &writer);
};


}


#endif //PLAYING_CLIENTMESSAGEHANDLER_H

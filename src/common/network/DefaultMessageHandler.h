//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_DEFAULTMESSAGEHANDLER_H
#define PLAYING_DEFAULTMESSAGEHANDLER_H

#include "MessageHandler.h"

class DefaultMessageHandler : public MessageHandler {
public:
    void handleMessage(MessageHandlingArgs& context);
    void debug(JsonWriter& writer);
};

#endif //PLAYING_DEFAULTMESSAGEHANDLER_H

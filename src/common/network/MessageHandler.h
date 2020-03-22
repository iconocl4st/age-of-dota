//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_MESSAGEHANDLER_H
#define PLAYING_MESSAGEHANDLER_H

#include "MessageTypes.h"
#include "MessageHandlingArgs.h"
#include "../json/JsonDebuggable.h"

class MessageHandler : public JsonDebuggable {
public:
	virtual ~MessageHandler() = default;

	virtual void handleMessage(MessageHandlingArgs& context) = 0;
};


#endif //PLAYING_MESSAGEHANDLER_H

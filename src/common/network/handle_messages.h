//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_MESSAGE_HANDLER_H
#define PLAYING_MESSAGE_HANDLER_H

#include <list>

#include "MessageHandler.h"

void handleNextMessage(
	std::list<MessageHandler *> handlers,
	MessageHandlingArgs &context
);

#endif //PLAYING_MESSAGE_HANDLER_H

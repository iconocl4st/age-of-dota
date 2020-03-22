
//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_MESSAGEHANDLINGARGS_H
#define PLAYING_MESSAGEHANDLINGARGS_H

#include <functional>

#include "../../common/util/logging_levels.h"
#include "../json/JsonReader.h"
#include "ConnectionContext.h"
#include "MessageTypes.h"

class MessageHandlingArgs {
public:
	aod::common::message::MessageType msgType;
	JsonReader& reader;
	bool stopHandlingMessages{false};
	bool messageIsHandled{false};
	ConnectionContext& connectionContext;
	Logger& logger;

	MessageHandlingArgs(
		JsonReader& reader,
		ConnectionContext& context,
		Logger& logger
	) :
		reader{reader},
		connectionContext{context},
		logger{logger} {}
};



#endif //PLAYING_MESSAGEHANDLINGARGS_H

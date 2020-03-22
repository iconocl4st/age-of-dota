//
// Created by thallock on 3/11/20.
//

#include "handle_messages.h"

void handleNextMessage(std::list<MessageHandler *> handlers, MessageHandlingArgs &context) {
	context.messageIsHandled = false;
	context.reader.readBeginObject();
	context.msgType = (aod::common::message::MessageType) context.reader.readInt("message-type");
	for (auto & handler : handlers) {
		handler->handleMessage(context);
		if (context.messageIsHandled) {
			break;
		}
	}
	if (!context.messageIsHandled) {
		std::ostringstream ss;
		ss << "Unhandled message!! Message type: " << aod::common::message::getMessageName(context.msgType);
		BOOST_LOG_SEV(context.logger, ERROR) << ss.str();
		// Might should just skip some stuff...
		throw std::runtime_error(ss.str());
	} else {
		context.reader.readEndObject();
	}

}
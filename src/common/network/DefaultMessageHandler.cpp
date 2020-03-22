//
// Created by thallock on 2/15/20.
//

#include "DefaultMessageHandler.h"

#include "ConnectionContext.h"
#include "MessageTypes.h"

void DefaultMessageHandler::handleMessage( MessageHandlingArgs& context) {
    switch (context.msgType) {
        case aod::common::message::NOOP:
			context.messageIsHandled = true;
        	break;
        case aod::common::message::CLOSE_CONNECTION:
        	context.stopHandlingMessages = true;
        	context.messageIsHandled = true;
        	break;
    }
}

void DefaultMessageHandler::debug(JsonWriter &writer) {
    writer.writeBeginObject();
    writer.writeString("type", "Default message handler");
    writer.writeEndObject();
}

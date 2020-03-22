//
// Created by thallock on 3/11/20.
//

#include "send_noop.h"

#include "MessageTypes.h"

void sendNoop(ConnectionContext& connection) {
	std::unique_lock<std::recursive_mutex> lock{connection.writeLock};

	connection.writer.writeBeginObject();
	connection.writer.writeInt("message-type", aod::common::message::NOOP);
	connection.writer.writeEndObject();
	connection.writer.flush();
}

void sendCloseConnection(ConnectionContext& context) {
	std::unique_lock<std::recursive_mutex> lock{context.writeLock};

	context.writer.writeBeginObject();
	context.writer.writeInt("message-type", aod::common::message::CLOSE_CONNECTION);
	context.writer.writeEndObject();
	context.writer.flush();
}
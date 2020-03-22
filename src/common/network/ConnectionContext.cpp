//
// Created by thallock on 2/15/20.
//

#include "ConnectionContext.h"
#include "MessageTypes.h"

void ConnectionContext::debug(JsonWriter &writer) {
    writer.writeBeginObject();
    writeAddress(this, writer);
    writer.writeString("name", name);
//    writer.writeBeginArray("handlers");
//    for (auto & handler : handlers) {
//        handler->debug(writer);
//    }
//    writer.writeEndArray();
    writer.writeEndObject();
}

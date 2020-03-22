//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_JSONSENDABLE_H
#define PLAYING_JSONSENDABLE_H

#include "JsonWriter.h"
#include "JsonReader.h"

class JsonSendable {
public:
    virtual void send(JsonWriter *writer) const = 0;
};

std::string printJsonSendable(JsonSendable *debuggable);

#endif //PLAYING_JSONSENDABLE_H

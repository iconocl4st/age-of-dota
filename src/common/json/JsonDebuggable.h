//
// Created by thallock on 3/8/20.
//

#ifndef PLAYING_JSONDEBUGGABLE_H
#define PLAYING_JSONDEBUGGABLE_H


#include <sstream>
#include "JsonWriter.h"

class JsonDebuggable {
protected:
    template <typename TYPE> void writeAddress(TYPE *t, JsonWriter& writer) {
        std::ostringstream pointerString;
        pointerString << t;
        writer.writeString("address", pointerString.str());
    }
public:
    virtual void debug(JsonWriter& writer) = 0;
};

std::string printJsonDebuggable(JsonDebuggable *debuggable);


#endif //PLAYING_JSONDEBUGGABLE_H

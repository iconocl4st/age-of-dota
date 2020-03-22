//
// Created by thallock on 3/8/20.
//

#include "JsonDebuggable.h"
#include "JsonTextWriter.h"

std::string printJsonDebuggable(JsonDebuggable *debuggable) {
    std::ostringstream outStream;
    {
    	StreamOutputBufferWrapper b{outStream};
		TextJsonWriter writer{b};
        debuggable->debug(writer);
    }
    return outStream.str();
}

//
//template <typename TYPE>
//void JsonDebuggable::writeAddress(TYPE *t, JsonWriter& writer) {
//    std::ostringstream pointerString;
//    pointerString << t;
//    writer.writeString("address", pointerString.str());
//}

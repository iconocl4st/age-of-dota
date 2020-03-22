//
// Created by thallock on 3/11/20.
//

#include "JsonSendable.h"
#include "JsonTextWriter.h"


std::string printJsonSendable(JsonSendable *debuggable) {
	std::ostringstream outStream;
	{
		StreamOutputBufferWrapper b{outStream};
		TextJsonWriter writer{b};
		debuggable->send(&writer);
	}
	return outStream.str();
}
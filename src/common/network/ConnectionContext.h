//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_CONNECTIONCONTEXT_H
#define PLAYING_CONNECTIONCONTEXT_H

#include <mutex>
#include <map>

#include "../json/JsonDebuggable.h"

class ConnectionContext : public JsonDebuggable {
public:
    std::string name;

	std::recursive_mutex writeLock;
	JsonWriter& writer;

	std::map<std::string, std::string> attributes;

    ConnectionContext(const std::string& name, JsonWriter& writer) :
        name{name},
		writer{writer} {}

    void debug(JsonWriter& writer);
};


#endif //PLAYING_CONNECTIONCONTEXT_H

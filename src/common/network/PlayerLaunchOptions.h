//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_PLAYERLAUNCHOPTIONS_H
#define PLAYING_PLAYERLAUNCHOPTIONS_H

#include "../json/JsonSendable.h"
#include "../spec/GameSpecification.h"

class PlayerLaunchOptions : public JsonSendable {
public:
	int playerNumber{-1};
	int numPlayers{-1};
	bool isSpectator{false};

	std::shared_ptr<GameSpecification> spec;

	// starting location...

	// make a &
	void send(JsonWriter *writer) const;
	// make a &
	void parse(JsonReader *reader);
};

#endif //PLAYING_PLAYERLAUNCHOPTIONS_H

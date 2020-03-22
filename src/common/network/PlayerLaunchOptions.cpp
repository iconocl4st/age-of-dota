//
// Created by thallock on 3/11/20.
//

#include "PlayerLaunchOptions.h"

void PlayerLaunchOptions::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeKey("spec");
	spec->send(writer);
	writer->writeInt("player-number", playerNumber);
	writer->writeBoolean("spectating", isSpectator);
	writer->writeInt("num-players", numPlayers);
	writer->writeEndObject();
}

void PlayerLaunchOptions::parse(JsonReader *reader) {
	reader->readBeginObject();
	reader->readKey("spec");

	reader->readBeginObject();
	int width = reader->readInt("width");
	int height = reader->readInt("height");
	reader->readEndObject();

	spec = createGameSpec(width, height);
	playerNumber = reader->readInt("player-number");
	isSpectator = reader->readBoolean("spectating");
	numPlayers = reader->readInt("num-players");
	reader->readEndObject();
}

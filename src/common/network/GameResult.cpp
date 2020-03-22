//
// Created by thallock on 3/16/20.
//

#include "GameResult.h"

void GameResult::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeInt("winning-player-number", winnerPlayerNumber);
	writer->writeEndObject();
}

void GameResult::parse(JsonReader *reader) {
	reader->readBeginObject();
	winnerPlayerNumber = reader->readInt("winning-player-number");
	reader->readEndObject();
}

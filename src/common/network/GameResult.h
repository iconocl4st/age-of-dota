//
// Created by thallock on 3/16/20.
//

#ifndef PLAYING_GAMERESULT_H
#define PLAYING_GAMERESULT_H


#include "../json/JsonSendable.h"

class GameResult : public JsonSendable {
public:
	int winnerPlayerNumber;

	void send(JsonWriter *writer) const;
	void parse(JsonReader *reader);

};


#endif //PLAYING_GAMERESULT_H

//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_NAVIGATE_LOBBY_H
#define PLAYING_NAVIGATE_LOBBY_H

#include "../common/util/logging_levels.h"

#include "../common/network/ConnectionContext.h"
#include "../common/json/JsonReader.h"
#include "../common/network/PlayerLaunchOptions.h"

bool navigate_lobby(
	const std::string &lobbyName,
	int playerNumber,
	ConnectionContext &connection,
	JsonReader &reader,
	PlayerLaunchOptions &options,
	Logger& mt
);

#endif //PLAYING_NAVIGATE_LOBBY_H

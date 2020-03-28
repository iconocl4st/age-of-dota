//
// Created by thallock on 2/14/20.
//

#ifndef PLAYING_MESSAGETYPES_H
#define PLAYING_MESSAGETYPES_H

#include <string>

namespace aod::common::message {

enum MessageType {
	// common
	NOOP,
	CLOSE_CONNECTION,

	// Client lobby
	LIST_LOBBIES,
	CREATE_LOBBY,
	JOIN_LOBBY,

	REQUEST_STATUS_UPDATE,
	SET_NUM_LOBBY_PLAYERS,
	SET_PLAYER_TO_AI,
	KICK_PLAYER,
	SET_PLAYER_TO_SPECTATOR,
	MARK_READY,

	LEAVE_LOBBY,
	LAUNCH,

	// Server lobby
	LOBBIES_LIST,
	LOBBY_STATUS_UPDATE,
	LAUNCHED,

	// Server game
	CREATE_ENTITY,
	DELETE_ENTITY,

	ENTITY_RECEIVED_DAMAGE,
	MOVEMENT_CHANGED,
	ENTITY_ACTION_CHANGED,

	PROJECTILE_LAUNCHED,
	PROJECTILE_DESTROYED,
	UPDATE_PROJECTILE,

	REQUEST_AI_ACTIONS,
	SERVER_TICK_BEGIN,
	SERVER_TICK_END,

	GAME_OVER,

	// Client actions
	REQUEST_ACTION,

	AI_ACTIONS_SENT,

	// Other
	CLOSE_SERVER,
};

// for server messages...
// client_messages
// client message handler
// handle_game_messages
// server game messages

std::string getMessageName(common::message::MessageType type);

void exportMessages(const std::string& fileName);

}


#endif //PLAYING_MESSAGETYPES_H

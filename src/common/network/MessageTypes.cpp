//
// Created by thallock on 2/15/20.
//

#include "MessageTypes.h"
#include "BufferWrapper.h"
#include "../json/JsonTextWriter.h"

#include <sstream>
#include <fstream>

namespace aod::common::message {

std::string getMessageName(common::message::MessageType type) {
	switch (type) {
		case NOOP:
			return "NOOP";
		case CLOSE_CONNECTION:
			return "CLOSE_CONNECTION";
		case LIST_LOBBIES:
			return "LIST_LOBBIES";
		case CREATE_LOBBY:
			return "CREATE_LOBBY";
		case JOIN_LOBBY:
			return "JOIN_LOBBY";
		case REQUEST_STATUS_UPDATE:
			return "REQUEST_STATUS_UPDATE";
		case SET_NUM_LOBBY_PLAYERS:
			return "SET_NUM_LOBBY_PLAYERS";
		case SET_PLAYER_TO_AI:
			return "SET_PLAYER_TO_AI";
		case KICK_PLAYER:
			return "KICK_PLAYER";
		case SET_PLAYER_TO_SPECTATOR:
			return "SET_PLAYER_TO_SPECTATOR";
		case MARK_READY:
			return "MARK_READY";
		case LEAVE_LOBBY:
			return "LEAVE_LOBBY";
		case LAUNCH:
			return "LAUNCH";
		case LOBBIES_LIST:
			return "LOBBIES_LIST";
		case LOBBY_STATUS_UPDATE:
			return "LOBBY_STATUS_UPDATE";
		case LAUNCHED:
			return "LAUNCHED";
		case CREATE_ENTITY:
			return "CREATE_ENTITY";
		case DELETE_ENTITY:
			return "DELETE_ENTITY";
		case ENTITY_RECEIVED_DAMAGE:
			return "ENTITY_RECEIVED_DAMAGE";
		case MOVEMENT_CHANGED:
			return "MOVEMENT_CHANGED";
		case ENTITY_ACTION_CHANGED:
			return "ENTITY_ACTION_CHANGED";
		case PROJECTILE_LAUNCHED:
			return "PROJECTILE_LAUNCHED";
		case PROJECTILE_DESTROYED:
			return "PROJECTILE_DESTROYED";
		case UPDATE_PROJECTILE:
			return "UPDATE_PROJECTILE";
		case REQUEST_AI_ACTIONS:
			return "REQUEST_AI_ACTIONS";
		case SERVER_TICK_BEGIN:
			return "SERVER_TICK_BEGIN";
		case SERVER_TICK_END:
			return "SERVER_TICK_END";
		case GAME_OVER:
			return "GAME_OVER";
		case REQUEST_ACTION:
			return "REQUEST_ACTION";
		case AI_ACTIONS_SENT:
			return "AI_ACTIONS_SENT";
		case CLOSE_SERVER:
			return "CLOSE_SERVER";
		default:
			std::ostringstream ss;
			ss << "Unknown (" << type << ")";
			return ss.str();
	}
}

void exportMessages(const std::string& fileName) {
	std::ofstream output;
	output.open(fileName);

	StreamOutputBufferWrapper streamBuffer{output};
	TextJsonWriter writer{streamBuffer};

	writer.writeBeginObject();
	writer.writeBeginObject("message-types");

	writer.writeInt(getMessageName(NOOP), NOOP);
	writer.writeInt(getMessageName(CLOSE_CONNECTION), CLOSE_CONNECTION);
	writer.writeInt(getMessageName(LIST_LOBBIES), LIST_LOBBIES);
	writer.writeInt(getMessageName(CREATE_LOBBY), CREATE_LOBBY);
	writer.writeInt(getMessageName(JOIN_LOBBY), JOIN_LOBBY);
	writer.writeInt(getMessageName(REQUEST_STATUS_UPDATE), REQUEST_STATUS_UPDATE);
	writer.writeInt(getMessageName(SET_NUM_LOBBY_PLAYERS), SET_NUM_LOBBY_PLAYERS);
	writer.writeInt(getMessageName(SET_PLAYER_TO_AI), SET_PLAYER_TO_AI);
	writer.writeInt(getMessageName(KICK_PLAYER), KICK_PLAYER);
	writer.writeInt(getMessageName(SET_PLAYER_TO_SPECTATOR), SET_PLAYER_TO_SPECTATOR);
	writer.writeInt(getMessageName(MARK_READY), MARK_READY);
	writer.writeInt(getMessageName(LEAVE_LOBBY), LEAVE_LOBBY);
	writer.writeInt(getMessageName(LAUNCH), LAUNCH);
	writer.writeInt(getMessageName(LOBBIES_LIST), LOBBIES_LIST);
	writer.writeInt(getMessageName(LOBBY_STATUS_UPDATE), LOBBY_STATUS_UPDATE);
	writer.writeInt(getMessageName(LAUNCHED), LAUNCHED);
	writer.writeInt(getMessageName(CREATE_ENTITY), CREATE_ENTITY);
	writer.writeInt(getMessageName(DELETE_ENTITY), DELETE_ENTITY);
	writer.writeInt(getMessageName(ENTITY_RECEIVED_DAMAGE), ENTITY_RECEIVED_DAMAGE);
	writer.writeInt(getMessageName(MOVEMENT_CHANGED), MOVEMENT_CHANGED);
	writer.writeInt(getMessageName(ENTITY_ACTION_CHANGED), ENTITY_ACTION_CHANGED);
	writer.writeInt(getMessageName(PROJECTILE_LAUNCHED), PROJECTILE_LAUNCHED);
	writer.writeInt(getMessageName(PROJECTILE_DESTROYED), PROJECTILE_DESTROYED);
	writer.writeInt(getMessageName(UPDATE_PROJECTILE), UPDATE_PROJECTILE);
	writer.writeInt(getMessageName(REQUEST_AI_ACTIONS), REQUEST_AI_ACTIONS);
	writer.writeInt(getMessageName(SERVER_TICK_BEGIN), SERVER_TICK_BEGIN);
	writer.writeInt(getMessageName(SERVER_TICK_END), SERVER_TICK_END);
	writer.writeInt(getMessageName(GAME_OVER), GAME_OVER);
	writer.writeInt(getMessageName(REQUEST_ACTION), REQUEST_ACTION);
	writer.writeInt(getMessageName(AI_ACTIONS_SENT), AI_ACTIONS_SENT);
	writer.writeInt(getMessageName(CLOSE_SERVER), CLOSE_SERVER);
	writer.writeEndObject();
	writer.writeEndObject();
}


}
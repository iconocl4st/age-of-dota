//
// Created by thallock on 3/11/20.
//

#include "navigate_lobby.h"

#include "../common/network/ConnectionContext.h"
#include "../common/json/JsonTextReader.h"
#include "../client_common/network/client_messages.h"
#include "../common/network/PlayerLaunchOptions.h"
#include "../common/network/MessageTypes.h"

#include <sstream>

namespace lobby_state {

enum LobbyState {
	REQUESTED_LOBBY_LIST,
	REQUESTED_MARK_READY,
	READY_TO_LAUNCH,
};

}


bool navigate_lobby(
	const std::string &lobbyName,
	int playerNumber,
	ConnectionContext &connection,
	JsonReader &reader,
	PlayerLaunchOptions &options,
	Logger& logger)
{

	BOOST_LOG_SEV(logger, NORMAL) << "Script: navigating lobby, lobby name = " <<
		lobbyName << ", player number = " << playerNumber;
	lobby_state::LobbyState currentState = lobby_state::REQUESTED_LOBBY_LIST;
	while (true) {
		reader.readBeginObject();
		auto msgType = (aod::common::message::MessageType) reader.readInt("message-type");
		BOOST_LOG_SEV(logger, DEBUG) << "Lobby loop: Received message of type " <<  aod::common::message::getMessageName(msgType);
		switch (msgType) {
			case aod::common::message::NOOP: {
				break;
			}
			case aod::common::message::LOBBIES_LIST: {
				if (currentState != lobby_state::REQUESTED_LOBBY_LIST) {
					BOOST_LOG_SEV(logger, ERROR) << "Invalid state";
					return false;
				}
				bool hasLobby = false;
				reader.readBeginArray("lobbies");
				while (!reader.atArrayEnd(json_token::BEGIN_OBJECT)) {
					if (reader.readString("lobby-name") == lobbyName) {
						hasLobby = true;
					}
					reader.readEndObject();
				}
				if (!hasLobby) {
					BOOST_LOG_SEV(logger, ERROR) << "Unable to find the lobby";
					return false;
				}

				aod::client::common::network::joinLobby(connection, false, lobbyName, playerNumber);
				aod::client::common::network::setReady(connection, true);
				currentState = lobby_state::REQUESTED_MARK_READY;
				break;
			}
			case aod::common::message::LOBBY_STATUS_UPDATE: {
				LobbyBrowserConnection bConnection;
				aod::client::common::network::parseLobbyConnection(reader, bConnection);
				if (bConnection.lobbyName != lobbyName) {
					BOOST_LOG_SEV(logger, ERROR) << "Joined the wrong lobby";
					return false;
				}
				if (!bConnection.connected) {
					break;
				}
				if (!bConnection.ready) {
					if (currentState == lobby_state::READY_TO_LAUNCH) {
						BOOST_LOG_SEV(logger, ERROR) << "No longer ready";
						return false;
					}
				}
				currentState = lobby_state::READY_TO_LAUNCH;
				break;
			}
			case aod::common::message::CLOSE_CONNECTION: {
				BOOST_LOG_SEV(logger, ERROR) << "Early close";
				return false;
			}
			case aod::common::message::LAUNCHED: {
				reader.readKey("launch-options");
				options.parse(&reader);
				reader.readEndObject();
				return true;
			}
			default: {
				BOOST_LOG_SEV(logger, ERROR) << "Unrecognized message type.";
				return false;
			}
		}
		reader.readEndObject();
	}
}

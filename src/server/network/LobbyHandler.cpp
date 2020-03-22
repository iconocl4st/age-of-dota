//
// Created by thallock on 2/15/20.
//

#include "LobbyHandler.h"
#include "../app/ServerContext.h"


void LobbyHandler::handleMessage(MessageHandlingArgs& args) {
	std::shared_ptr<Lobby> lobby{nullptr};
	auto it = args.connectionContext.attributes.find("lobby");
	if (it != args.connectionContext.attributes.end()) {
		lobby = serverContext.findLobby(it->second);
	}

    switch (args.msgType) {
    	case aod::common::message::LAUNCH:
    		if (lobby != nullptr) {
				serverContext.launchGame(lobby.get());
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "Launch with no lobby present";
    		}
            args.messageIsHandled = true;
            break;
    	case aod::common::message::MARK_READY: {
            bool ready = args.reader.readBoolean("ready");
			if (lobby != nullptr) {
				lobby->setPlayerReady(args.connectionContext.name, ready);
				lobby->updateClients();
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "Ready with no lobby present";
			}
			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::LEAVE_LOBBY:
        	// TODO
            lobby->updateClients();
			args.messageIsHandled = true;
			break;
        case aod::common::message::SET_NUM_LOBBY_PLAYERS: {
            int numPlayers = args.reader.readInt("number-of-players");
			if (lobby != nullptr) {
				lobby->setNumPlayers(numPlayers);
				lobby->updateClients();
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "Set num players with no lobby present";
			}
			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::SET_PLAYER_TO_AI: {
            int playerNumber = args.reader.readInt("player-number");
            bool isAi = args.reader.readBoolean("is-ai");
			if (lobby != nullptr) {
				lobby->setPlayerAsAi(playerNumber, isAi);
				lobby->updateClients();
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "Set to ai with no lobby present";
			}
			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::SET_PLAYER_TO_SPECTATOR: {
            bool isSpectator = args.reader.readBoolean("spectating");
			if (lobby != nullptr) {
				lobby->setPlayerAsSpectator(args.connectionContext.name, isSpectator);
				lobby->updateClients();
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "spectate with no lobby present";
			}
			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::KICK_PLAYER: {
        	// TODO
			if (lobby != nullptr) {
				lobby->updateClients();
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "Kick with no lobby present";
			}
			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::REQUEST_STATUS_UPDATE: {
			if (lobby != nullptr) {
				sendFilledStatusUpdate(lobby.get(), &args.connectionContext);
			} else {
				BOOST_LOG_SEV(serverContext.logger, ERROR) << "Status update request with no lobby present";
			}
			args.messageIsHandled = true;
			break;
        }
    }
}

void LobbyHandler::debug(JsonWriter &writer) {
    writer.writeBeginObject();
    writer.writeString("type", "Lobby handler");
//    writer.writeString("lobby-name", lobbyName);
    writer.writeEndObject();
}

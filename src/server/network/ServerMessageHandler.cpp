//
// Created by thallock on 2/15/20.
//

#include <memory>

#include "ServerMessageHandler.h"
#include "../app/ServerContext.h"
#include "../../common/network/MessageTypes.h"

void ServerMessageHandler::handleMessage(MessageHandlingArgs& args) {
    switch (args.msgType) {
		case aod::common::message::CLOSE_CONNECTION:
		{
			args.messageIsHandled = true;
			args.stopHandlingMessages = true;
			break;
		}
        case aod::common::message::CREATE_LOBBY:
        {
            std::string name = args.reader.readString("lobby-name");
            serverContext.createLobby(name);

            args.messageIsHandled = true;
            break;
        }
        case aod::common::message::JOIN_LOBBY:
        {
            std::string name = args.reader.readString("lobby-name");
            bool isHuman = args.reader.readBoolean("is-human");
            int playerNumber = args.reader.readInt("player-number");

            std::unique_lock<std::recursive_mutex> lock{serverContext.lobbiesMutex};
            std::shared_ptr<Lobby> lobby = serverContext.findLobby(name);
            if (lobby) {
                lobby->addPlayer(&args.connectionContext, isHuman ? HUMAN : AI, playerNumber);
                args.connectionContext.attributes["lobby"] = lobby->name;
                lobby->updateClients();
            }
			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::LEAVE_LOBBY:
        {
        	args.connectionContext.attributes.erase("lobby");
			args.messageIsHandled = true;
			// cancel the lobby if there is no one else?...
			break;
        }
        case aod::common::message::REQUEST_STATUS_UPDATE:
        {
            std::unique_lock<std::recursive_mutex> lock{args.connectionContext.writeLock};
            args.connectionContext.writer.writeBeginObject();
            args.connectionContext.writer.writeInt("message-type", aod::common::message::LOBBY_STATUS_UPDATE);
            args.connectionContext.writer.writeBoolean("connected", false);
            args.connectionContext.writer.writeBoolean("ready", false);
            args.connectionContext.writer.writeBoolean("all-ready", false);
            args.connectionContext.writer.writeBoolean("spectating", false);
            args.connectionContext.writer.writeString("lobby-name", "");
            args.connectionContext.writer.writeEndObject();
            args.connectionContext.writer.flush();

			args.messageIsHandled = true;
			break;
        }
        case aod::common::message::LIST_LOBBIES:
        {
            std::unique_lock<std::recursive_mutex> lock{args.connectionContext.writeLock};

            args.connectionContext.writer.writeBeginObject();
			args.connectionContext.writer.writeInt("message-type", aod::common::message::LOBBIES_LIST);
			args.connectionContext.writer.writeBeginArray("lobbies");

            {
                std::unique_lock<std::recursive_mutex> llock{serverContext.lobbiesMutex};
                for (auto &lobby : serverContext.lobbies) {
					args.connectionContext.writer.writeBeginObject();
					args.connectionContext.writer.writeString("lobby-name", lobby->name);
					args.connectionContext.writer.writeEndObject();
                }
            }

			args.connectionContext.writer.writeEndArray();
			args.connectionContext.writer.writeEndObject();
			args.connectionContext.writer.flush();

			args.messageIsHandled = true;
			break;
        }
    }
}

void ServerMessageHandler::debug(JsonWriter &writer) {
    writer.writeBeginObject();
    writer.writeString("type", "Server message handler");
    writer.writeEndObject();
}

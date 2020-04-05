//
// Created by thallock on 2/15/20.
//

#include "ClientMessageHandler.h"
#include "../app/UiClientContext.h"
#include "../../common/network/MessageTypes.h"
#include "../../client_common/network/client_messages.h"
#include "../../common/network/send_noop.h"

#include <set>


namespace aod::client::app {

ClientMessageHandler::ClientMessageHandler(UiClientContext &context) : clientContext{context} {}

void ClientMessageHandler::handleMessage(MessageHandlingArgs& context) {
    switch (context.msgType) {
        case aod::common::message::LOBBIES_LIST:
        {
            std::set<std::string> lobbyNames;

			context.reader.readBeginArray("lobbies");
            while (!context.reader.atArrayEnd(json_token::BEGIN_OBJECT)) {
                lobbyNames.insert(context.reader.readString("lobby-name"));
				context.reader.readEndObject();
            }
            clientContext.lobbyBrowser.setLobbies(lobbyNames);
            context.messageIsHandled = true;
            break;
        }
        case aod::common::message::LOBBY_STATUS_UPDATE:
        {
            LobbyBrowserConnection connection;

			aod::client::common::network::parseLobbyConnection(context.reader, connection);

            clientContext.lobbyBrowser.setConnection(connection);

            if (connection.allReady && clientContext.onReady) {
				clientContext.onReady.value()(clientContext);
			}
            context.messageIsHandled = true;
            break;
        }
        case aod::common::message::LAUNCHED:
        {
            PlayerLaunchOptions options;

			context.reader.readKey("launch-options");
            options.parse(&context.reader);

            clientContext.launchGame(options);

            context.messageIsHandled = true;
            break;
        }
        case aod::common::message::CREATE_ENTITY:
        {
			aod::client::common::network::parseCreateEntityBody(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
            context.messageIsHandled = true;
            break;
        }
		case aod::common::message::ENTITY_ACTION_CHANGED:
		{
			aod::client::common::network::parseActionChanged(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::SERVER_TICK_BEGIN: // TODO: Should this happen at the end of the tick...
		{
			aod::client::common::network::parseTickBegin(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			clientContext.getGameState()->clientGameState.sharedState.updateLocations();
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::SERVER_TICK_END: // TODO: Should this happen at the end of the tick...
		{
			aod::client::common::network::parseTickEnd(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			clientContext.getGameState()->clientGameState.sharedState.updateLocations();
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::MOVEMENT_CHANGED:
		{
			aod::client::common::network::parseMovementChanged(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);

			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::PROJECTILE_LAUNCHED:
		{
			aod::client::common::network::parseProjectileLaunch(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::UPDATE_PROJECTILE:
		{
			aod::client::common::network::parseProjectileUpdate(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			context.messageIsHandled = true;
			break;
		}
    	case aod::common::message::PROJECTILE_DESTROYED:
		{
			aod::client::common::network::parseProjectileDestroy(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::ENTITY_RECEIVED_DAMAGE:
		{
			aod::client::common::network::parseEntityReceivedDamage(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::DELETE_ENTITY:
		{
			aod::client::common::network::parseDeleteEntity(
				context.reader,
				clientContext.getGameState()->clientGameState.sharedState
			);
			context.messageIsHandled = true;
			break;
		}
    	case aod::common::message::GAME_OVER:
		{
			GameResult result;
			aod::client::common::network::parseGameEnd(context.reader, result);
			sendCloseConnection(context.connectionContext);
			clientContext.closeGame();
			context.messageIsHandled = true;
			break;
		}
		case aod::common::message::REQUEST_AI_ACTIONS:
		{
			aod::client::common::network::sendAiActionsSent(context.connectionContext);
			context.messageIsHandled = true;
			break;
		}
    	case aod::common::message::CLOSE_CONNECTION:
		{
			context.stopHandlingMessages = true;
			context.messageIsHandled = true;
			break;
		}
    }
}

void ClientMessageHandler::debug(JsonWriter &writer) {
    writer.writeBeginObject();
    writer.writeString("type", "Client message handler");
    writer.writeEndObject();
}

}


//
// Created by thallock on 3/8/20.
//

#include "ServerGameHandler.h"

#include "server_game_messages.h"
#include "../../common/state/Entity.h"

void ServerGameHandler::handleMessage(MessageHandlingArgs& args) {
	std::shared_ptr<ServerGameState> game{nullptr};

	auto it = args.connectionContext.attributes.find("game");
	if (it != args.connectionContext.attributes.end()) {
		game = serverContext.getGame(it->second);
	}

	switch (args.msgType) {
		case aod::common::message::REQUEST_ACTION: {
			// Validate this request first...

			EntityId entityId;
			std::shared_ptr<aod::common::action::Action> action;
			aod::server::message::parseRequestAction(args.reader, entityId, action);

			args.messageIsHandled = true;

			if (game == nullptr) {
				break;
			}
			Entity entity{entityId, game->sharedState};
			if (!entity.isLocked) {
				break;
			}
			auto player = entity.getPlayer();
			if (!player) {
				break;
			}
			int playerNumber = game->getPlayerNumber(args.connectionContext.name);
			if (playerNumber < 0) {
				break;
			}
			if (playerNumber != player.value()) {
				BOOST_LOG_SEV(serverContext.logger, WARN) << "Player tried to control non-owned entity";
				break;
			}
			game->incomingRequests.set(entityId, action);
			break;
		}
		case aod::common::message::AI_ACTIONS_SENT:
		{
			bool hasProjections = args.reader.readBoolean("has-projections");
			if (hasProjections) {
				args.reader.readDouble("expected-rewards");
				args.reader.readDouble("current-value");
			}

			if (game) {
				game->receivedAiReady(args.connectionContext.name);
			}
			args.messageIsHandled = true;
			break;
		}
	}
}

void ServerGameHandler::debug(JsonWriter &writer) {

}

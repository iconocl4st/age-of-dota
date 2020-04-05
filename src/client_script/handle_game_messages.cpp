//
// Created by thallock on 3/12/20.
//

#include "handle_game_messages.h"
#include "../common/network/MessageTypes.h"
#include "../common/state/state_manipulator.h"
#include "../client_common/network/client_messages.h"
#include "../common/network/send_noop.h"
#include "../common/state/Entity.h"


static void assignRandomAction(ScriptClientContext &clientContext, EntityId entityId) {
	Entity entity{entityId, clientContext.clientState.sharedState};
	if (!entity.isLocked) {
		return;
	}
	auto action = entity.getAction();
	if (action->getType() != aod::common::action::IDLE) {
		return;
	}
	auto playerNumber = entity.getPlayer().value();
	if (playerNumber != clientContext.clientState.playerNumber) {
		return;
	}
	if (clientContext.random.nextBool()) {

		assignMovement(clientContext, entityId);
	} else {
		aod::client::common::network::requestAction(
			clientContext.connectionContext,
			entityId,
			std::make_shared<aod::common::action::Attack>()
		);
	}
}

int handleGameMessages(JsonReader& reader, ScriptClientContext &clientContext) {
	BOOST_LOG_SEV(clientContext.logger, NORMAL) << "Handling game messages";

	bool keepConnectionOpen{true};
	while (keepConnectionOpen) {
		reader.readBeginObject();
		auto msgType = (aod::common::message::MessageType) reader.readInt("message-type");
		BOOST_LOG_SEV(clientContext.logger, DEBUG) << "Game loop: Received message of type " <<  aod::common::message::getMessageName(msgType);

		switch (msgType) {
			case aod::common::message::NOOP: {
				break;
			}
			case aod::common::message::CREATE_ENTITY: {
				EntityId entityId = aod::client::common::network::parseCreateEntityBody(
					reader,
					clientContext.clientState.sharedState
				);
//				clientContext.scriptAi.addEntity(clientContext, entityId);

				assignRandomAction(clientContext, entityId);
				break;
			}
			case aod::common::message::ENTITY_ACTION_CHANGED:
			{
				EntityId entityId = aod::client::common::network::parseActionChanged(
					reader,
					clientContext.clientState.sharedState
				);

				assignRandomAction(clientContext, entityId);
				break;
			}
			case aod::common::message::MOVEMENT_CHANGED:
			{
				aod::client::common::network::parseMovementChanged(reader, clientContext.clientState.sharedState);
				break;
			}
			case aod::common::message::SERVER_TICK_BEGIN:
			{
				aod::client::common::network::parseTickBegin(reader, clientContext.clientState.sharedState);
				break;
			}
			case aod::common::message::SERVER_TICK_END:
			{
				aod::client::common::network::parseTickEnd(reader, clientContext.clientState.sharedState);
				break;
			}
			case aod::common::message::PROJECTILE_LAUNCHED:
			{
				aod::client::common::network::parseProjectileLaunch(
					reader,
					clientContext.clientState.sharedState
				);
				break;
			}
			case aod::common::message::UPDATE_PROJECTILE:
			{
				aod::client::common::network::parseProjectileUpdate(
					reader,
					clientContext.clientState.sharedState
				);
				break;
			}
			case aod::common::message::PROJECTILE_DESTROYED:
			{
				aod::client::common::network::parseProjectileDestroy(
					reader,
					clientContext.clientState.sharedState
				);
				break;
			}
			case aod::common::message::ENTITY_RECEIVED_DAMAGE:
			{
				aod::client::common::network::parseEntityReceivedDamage(
					reader,
					clientContext.clientState.sharedState
				);
				break;
			}
			case aod::common::message::DELETE_ENTITY:
			{
				aod::client::common::network::parseDeleteEntity(
					reader,
					clientContext.clientState.sharedState
				);
				break;
			}
			case aod::common::message::CLOSE_CONNECTION:
			{
				keepConnectionOpen = false;
				break;
			}
			case aod::common::message::GAME_OVER:
			{
				GameResult result;
				aod::client::common::network::parseGameEnd(reader, result);
				sendCloseConnection(clientContext.connectionContext);
				break;
			}
			case aod::common::message::REQUEST_AI_ACTIONS:
			{
				aod::client::common::network::sendAiActionsSent(clientContext.connectionContext);
				break;
			}
			default: {
				BOOST_LOG_SEV(clientContext.logger, ERROR) << "Unrecognized message type: " << msgType;
				return -1;
			}
		}
		reader.readEndObject();
	}
	BOOST_LOG_SEV(clientContext.logger, DEBUG) << "Script client terminating";

	return 0;
}

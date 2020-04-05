//
// Created by thallock on 3/10/20.
//

#include "server_game_messages.h"
#include "../../common/network/MessageTypes.h"
#include "../../common/network/GameResult.h"

namespace aod::server::message {

void sendCreateEntity(ConnectionContext *connection, EntityId entityId, aod::common::state::EntityCreationParams *params) {
    std::unique_lock<std::recursive_mutex> lock{connection->writeLock};

    connection->writer.writeBeginObject();
	connection->writer.writeInt("message-type", aod::common::message::CREATE_ENTITY);
	connection->writer.writeInt("entity-id", entityId);
	connection->writer.writeKey("creation-params");
	params->send(&connection->writer);
	connection->writer.writeEndObject();
}

void sendApplyDamage(
	ConnectionContext *context,
	EntityId entityId,
	DamageInstance& instance,
	aod::common::state::DamageApplicationResult& result
) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};

	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::ENTITY_RECEIVED_DAMAGE);
	context->writer.writeInt("entity-id", entityId);
	context->writer.writeKey("damage");
	instance.send(&context->writer);
	context->writer.writeKey("result");
	result.send(&context->writer);
	context->writer.writeEndObject();
}

void sendDeleteEntity(ConnectionContext *connection, EntityId entityId) {
	std::unique_lock<std::recursive_mutex> lock{connection->writeLock};

	connection->writer.writeBeginObject();
	connection->writer.writeInt("message-type", aod::common::message::DELETE_ENTITY);
	connection->writer.writeInt("entity-id", entityId);
	connection->writer.writeEndObject();
}

void sendEntityChangedAction(
	ConnectionContext *context,
	EntityId entityId,
	std::shared_ptr<aod::common::action::Action> action,
	bool isRequested
) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};

	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::ENTITY_ACTION_CHANGED);
	context->writer.writeInt("entity-id", entityId);
	context->writer.writeBoolean("was-requested", isRequested);
	aod::common::action::sendAction("action", context->writer, action);
	context->writer.writeEndObject();
}

void sendChangeEntityMovement(ConnectionContext *context, EntityId entityId, Movement &movement) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};

	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::MOVEMENT_CHANGED);
	context->writer.writeInt("entity-id", entityId);
	context->writer.writeKey("movement");
	movement.send(&context->writer);
	context->writer.writeEndObject();
}

void sendTickBegin(ConnectionContext *context, GameTime gameTime, std::chrono::high_resolution_clock::time_point& tickTime) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};

	unsigned long long ns0 = tickTime.time_since_epoch().count();
	std::ostringstream timeThing;
	timeThing << ns0;

	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::SERVER_TICK_BEGIN);
	context->writer.writeDouble("current-time", gameTime);
	context->writer.writeString("tick-time", timeThing.str());
	context->writer.writeEndObject();
}

void sendTickEnd(ConnectionContext *context) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};
	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::SERVER_TICK_END);
	context->writer.writeEndObject();
}

void sendProjectileLaunch(ConnectionContext *context, WeaponFire& fire) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};

	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::PROJECTILE_LAUNCHED);
	context->writer.writeKey("projectile");
	fire.send(&context->writer);
	context->writer.writeEndObject();
}

void sendProjectileDestroy(ConnectionContext *context, ProjectileId id) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};
	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::PROJECTILE_DESTROYED);
	context->writer.writeInt("projectile", id);
	context->writer.writeEndObject();
}

void sendProjectileUpdate(ConnectionContext *context, ProjectileId id, int projectileNum) {
	std::unique_lock<std::recursive_mutex> lock{context->writeLock};
	context->writer.writeBeginObject();
	context->writer.writeInt("message-type", aod::common::message::UPDATE_PROJECTILE);
	context->writer.writeInt("projectile-id", id);
	context->writer.writeInt("projectile-num", projectileNum);
	context->writer.writeEndObject();
}

void sendGameOver(ConnectionContext& context, GameResult& result) {
	std::unique_lock<std::recursive_mutex> lock{context.writeLock};
	context.writer.writeBeginObject();
	context.writer.writeInt("message-type", aod::common::message::GAME_OVER);
	context.writer.writeKey("result");
	result.send(&context.writer);
	context.writer.writeEndObject();
}

void sendRequestAiInput(ConnectionContext& context) {
	std::unique_lock<std::recursive_mutex> lock{context.writeLock};
	context.writer.writeBeginObject();
	context.writer.writeInt("message-type", aod::common::message::REQUEST_AI_ACTIONS);
	context.writer.writeEndObject();
}

void parseRequestAction(JsonReader& reader, EntityId& entityId, std::shared_ptr<aod::common::action::Action>& action) {
	entityId = (EntityId) reader.readInt("entity-id");
	return aod::common::action::parseAction("action", reader, action);
}


}
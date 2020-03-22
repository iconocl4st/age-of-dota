//
// Created by thallock on 3/11/20.
//

#include "client_messages.h"
#include "../../common/network/MessageTypes.h"
#include "../../common/state/state_manipulator.h"
#include "../../common/state/LocationManager.h"
#include "../../common/state/Entity.h"

namespace aod::client::common::network {

void sendRefreshLobbiesRequest(ConnectionContext &connection) {
	std::unique_lock<std::recursive_mutex> lock{connection.writeLock};

	connection.writer.writeBeginObject();
	connection.writer.writeInt("message-type", aod::common::message::LIST_LOBBIES);
	connection.writer.writeEndObject();
	connection.writer.flush();
}

void setReady(ConnectionContext &connection, bool isReady) {
	std::unique_lock<std::recursive_mutex> lock{connection.writeLock};
	connection.writer.writeBeginObject();
	connection.writer.writeInt("message-type", aod::common::message::MARK_READY);
	connection.writer.writeBoolean("ready", isReady);
	connection.writer.writeEndObject();
	connection.writer.flush();
}

void joinLobby(ConnectionContext &connection, bool isHuman, const std::string &lobbyName, int playerNumber) {
	std::unique_lock<std::recursive_mutex> lock{connection.writeLock};
	connection.writer.writeBeginObject();
	connection.writer.writeInt("message-type", aod::common::message::JOIN_LOBBY);
	connection.writer.writeString("lobby-name", lobbyName);
	connection.writer.writeBoolean("is-human", isHuman);
	connection.writer.writeInt("player-number", playerNumber);
	connection.writer.writeEndObject();
	connection.writer.flush();
}


void requestAction(ConnectionContext& connection, EntityId entityId, std::shared_ptr<aod::common::action::Action> action) {
	std::unique_lock<std::recursive_mutex> lock{connection.writeLock};

	connection.writer.writeBeginObject();
	connection.writer.writeInt("message-type", aod::common::message::REQUEST_ACTION);
	connection.writer.writeInt("entity-id",  entityId);
	aod::common::action::sendAction("action", connection.writer, action);
	connection.writer.writeEndObject();
	connection.writer.flush();
}


void moveTo(ConnectionContext &connection, EntityId entityId, const std::list<Point>& path, double finalOrientation) {
	// TODO: Timestamp

	connection.writer.writeEndObject();
	connection.writer.flush();
}


void parseLobbyConnection(JsonReader& reader, LobbyBrowserConnection& connection) {
	connection.connected = reader.readBoolean("connected");
	connection.ready = reader.readBoolean("ready");
	connection.allReady = reader.readBoolean("all-ready");
	connection.isSpectating = reader.readBoolean("spectating");
	connection.lobbyName = reader.readString("lobby-name");
}

EntityId parseCreateEntityBody(JsonReader &reader, aod::common::state::CommonState& sharedState) {
	aod::common::state::EntityCreationParams params;

	EntityId entityId = reader.readInt("entity-id");
	std::function<EntityId()> getEntityId = [entityId]() -> EntityId { return entityId; };
	params.getEntityId = &getEntityId;

	reader.readKey("creation-params");
	params.parse(&reader, sharedState.specification);

	aod::common::state::create_entity(sharedState, params);

	return entityId;
}

void parseDeleteEntity(JsonReader &reader, aod::common::state::CommonState& sharedState) {
	aod::common::state::EntityCreationParams params;

	EntityId entityId = reader.readInt("entity-id");
	aod::common::state::destroy_entity(sharedState, entityId);
}

EntityId parseActionChanged(JsonReader &reader, aod::common::state::CommonState &sharedState) {
	EntityId entityId = reader.readInt("entity-id");
	std::shared_ptr<aod::common::action::Action> a;
	aod::common::action::parseAction("action", reader, a);
	aod::common::state::setAction(sharedState, entityId, a);
	return entityId;
}

EntityId parseMovementChanged(JsonReader &reader, aod::common::state::CommonState &sharedState) {
	EntityId entityId;
	Movement movement;

	entityId = reader.readInt("entity-id");
	reader.readKey("movement");
	movement.parse(&reader);

	// lock the entity?
	sharedState.movements[entityId] = movement;

	return entityId;
}

void parseProjectileLaunch(JsonReader &reader, aod::common::state::CommonState &sharedState) {
	WeaponFire fire;

	reader.readKey("projectile");
	fire.parse(&reader);

	sharedState.attacks[fire.id] = fire;
}

void parseProjectileUpdate(JsonReader& reader, aod::common::state::CommonState&  sharedState) {
	auto id = (ProjectileId) reader.readInt("projectile-id");
	int projectileNum = reader.readInt("projectile-num");
	sharedState.attacks[id].sunkProjectiles.insert(projectileNum);
}
void parseProjectileDestroy(JsonReader& reader, aod::common::state::CommonState& sharedState) {
	auto id = (ProjectileId) reader.readInt("projectile");

	sharedState.attacks.erase(id);
}

void parseEntityReceivedDamage(JsonReader& reader, aod::common::state::CommonState& sharedState) {
	DamageInstance instance;
	aod::common::state::DamageApplicationResult result;
	auto entityId = (ProjectileId) reader.readInt("entity-id");
	reader.readKey("damage");
	instance.parse(&reader);
	reader.readKey("result");
	result.parse(&reader);

	aod::common::state::apply_damage(sharedState, entityId, instance);
}

void parseGameTime(JsonReader &reader, aod::common::state::CommonState &sharedState) {
	GameTime time = reader.readDouble("current-time");
	std::string timeString = reader.readString("tick-time");

	std::istringstream in{timeString};
	unsigned long long ns0;
	in >> ns0;
	std::chrono::high_resolution_clock::duration d(ns0);
	std::chrono::high_resolution_clock::time_point tickTime(d);

//	"Expected: "  << sharedState.interpolateTime(tickTime) << ", found: " << time;

	sharedState.currentTime = time;
	sharedState.lastTickTime = tickTime;
}

void parseGameEnd(JsonReader &reader, GameResult &result) {
	reader.readKey("result");
	result.parse(&reader);
}


void sendAiActionsSent(ConnectionContext& context) {
	std::unique_lock<std::recursive_mutex> lock{context.writeLock};
	context.writer.writeBeginObject();
	context.writer.writeInt("message-type", aod::common::message::AI_ACTIONS_SENT);
	context.writer.writeEndObject();
	context.writer.flush();
}


}

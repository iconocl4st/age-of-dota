//
// Created by thallock on 3/7/20.
//

#include <functional>
#include "ServerGameState.h"
#include "../app/ServerContext.h"
#include "../../common/state/Entity.h"
#include "../network/server_game_messages.h"


ServerGameState::ServerGameState(const std::string& uuid, LaunchOptions &options) :
    uuid{uuid},
    sharedState{std::shared_ptr<GameSpecification>{options.createGameSpecification()}} {}

ServerGameState::~ServerGameState() {
	for (auto& it : aiThreads) {
		it->join();
	}
}


void ServerGameState::debug(JsonWriter &writer) {
    writer.writeBeginObject();
    writer.writeString("uuid", uuid);
	writer.writeInt("current-time", sharedState.currentTime);
	// TODO: locks

    writer.writeBeginArray("connections");
    for (auto & pair : playerConnections) {
		writer.writeString(pair.second->name);
    }
    writer.writeEndArray();

	writer.writeBeginArray("spectators");
	for (auto & spectator : spectatorConnections) {
		writer.writeString(spectator->name);
	}
	writer.writeEndArray();

	writer.writeBeginArray("entities");
	for (auto & entityId : sharedState.entityIds) {
		Entity entity{entityId, sharedState};
		if (!entity.isLocked) {
			continue;
		}
		entity.debug(writer);
	}
	writer.writeEndArray();

    writer.writeEndObject();
}

int ServerGameState::getNumPlayers() {
    return 1 + playerConnections.size();
}

void ServerGameState::broadcast(std::function<void(ConnectionContext&)> f) {
    for (auto & p : playerConnections) {
        f(*p.second);
    }
    for (auto & s : spectatorConnections) {
		f(*s);
    }
}
void ServerGameState::broadcastToPlayers(std::function<void(ConnectionContext&)> f) {
	for (auto & p : playerConnections) {
		f(*p.second);
	}
}

void ServerGameState::flushAll() {
	broadcast([](ConnectionContext& w) -> void {
		std::unique_lock<std::recursive_mutex> lock{w.writeLock};
		w.writer.flush();
	});
}

EntityId ServerGameState::generateEntityId() {
    EntityId entityId;
    do {
        entityId = entityIdGenerator();
    } while (sharedState.entityIds.find(entityId) != sharedState.entityIds.end());
    return entityId;
}

void ServerGameState::receivedAiReady(const std::string& connectionName) {
	std::unique_lock lock{tickCompleteMutex};
	tickCompleteConnections.insert(connectionName);
	tickCompleteCv.notify_all();

}

void ServerGameState::requestAiActions() {
	std::unique_lock lock{tickCompleteMutex};
	tickCompleteConnections.clear();
	for (auto & p : playerConnections) {
		aod::server::message::sendRequestAiInput(*p.second);
	}
}

void ServerGameState::waitUntilAllReady() {
	std::unique_lock lock{tickCompleteMutex};
	tickCompleteCv.wait(lock, [this]()-> bool {
		for (auto& p : playerConnections) {
			if (tickCompleteConnections.find(p.second->name) == tickCompleteConnections.end()) {
				return false;
			}
		}
		return true;
	});
}

int ServerGameState::getPlayerNumber(const std::string& connectionName) {
	for (auto & it : playerConnections) {
		if (it.second->name == connectionName) {
			return it.first;
		}
	}
	return -1;
}
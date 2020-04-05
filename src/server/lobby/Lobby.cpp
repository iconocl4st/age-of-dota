//
// Created by thallock on 2/15/20.
//


#include <iostream>
#include "Lobby.h"
#include "launch_ai.h"
#include "../../common/network/MessageTypes.h"


#define INITIAL_NUM_SLOTS 2

std::function<void(const std::string&, int)> aiLauncher;

void setLobbyAiMethod(std::function<void(const std::string&, int)> m) {
	aiLauncher = m;
}

bool Lobby::isPlayerReady(const std::string &playerName) {
    std::unique_lock<std::recursive_mutex> lock{mutex};
    if (isPlayerSpectating(playerName)) {
        return true;
    }
    auto slot = getSlot(playerName);
    if (slot == nullptr) {
        return false;
    }
    return slot->isReady();
}

bool Lobby::areAllPlayersReady() {
    std::unique_lock<std::recursive_mutex> lock{mutex};
    for (auto & slot : playerSlots) {
        if (!slot->isReady()) {
            return false;
        }
    }
    return true;
}

void Lobby::setPlayerReady(const std::string &playerName, bool ready) {
    std::unique_lock<std::recursive_mutex> lock{mutex};
    if (isPlayerSpectating(playerName)) {
		BOOST_LOG_SEV(logger, ERROR) << "Cannot set a spectator's ready status";
        return;
    }
    auto slot = getSlot(playerName);
    if (slot == nullptr) {
        return;
    }
    slot->setReady(ready);
}

void Lobby::addPlayer(ConnectionContext *pContext, PlayerType type, int playerNumber) {
    std::unique_lock<std::recursive_mutex> lock{mutex};

    PlayerSlot *nextSlot{nullptr};

    // TODO: Should check if the user is already here...

    if (playerNumber >= 0) {
        if (playerNumber >= playerSlots.size()) {
			BOOST_LOG_SEV(logger, ERROR) << "Invalid index into players: " << playerNumber;
            return;
        }
        nextSlot = playerSlots[playerNumber].get();
        if (nextSlot->getType() != EMPTY) {
			BOOST_LOG_SEV(logger, ERROR) << "Slot already assigned: " << playerNumber;
            return;
        }
    } else {
        bool foundSlot = false;
        for (auto & slot : playerSlots) {
            if (slot->getType() != EMPTY) {
                continue;
            }
            nextSlot = slot.get();
            foundSlot = true;
            break;
        }

        if (!foundSlot) {
			BOOST_LOG_SEV(logger, ERROR) << "Unable to add player...";
        }
    }

    nextSlot->setType(type);
    nextSlot->setConnection(pContext);
}

void Lobby::setNumPlayers(int numPlayers) {
    std::unique_lock<std::recursive_mutex> lock{mutex};

    int missingPlayerSlots = numPlayers - playerSlots.size();
    if (missingPlayerSlots == 0) {
        return;
    }
    if (missingPlayerSlots < 0) {
		BOOST_LOG_SEV(logger, ERROR) << "Implement removing players...";
        return;
    }

    while (playerSlots.size() < numPlayers) {
        playerSlots.push_back(std::make_shared<PlayerSlot>());
    }
}

void Lobby::setPlayerAsAi(int playerNumber, bool isAi, AiType aiType) {
    std::unique_lock<std::recursive_mutex> lock{mutex};

    if (playerNumber < 0 || playerNumber >= playerSlots.size()) {
		BOOST_LOG_SEV(logger, ERROR) << "Player index out of bounds";
        return;
    }
    if (playerSlots[playerNumber]->getType() != EMPTY) {
		BOOST_LOG_SEV(logger, ERROR) << "Only know how to set empty player to ai";
        return;
    }
    if (isAi) {
    	// TODO: We should accept another parameter...
		if (aiType == SCRIPT_AI) {
			BOOST_LOG_SEV(logger, NORMAL) << "Launching native AI thread...";
			aiThreads.push_back(std::make_shared<std::thread>(aiLauncher, name, playerNumber));
		} else if (aiType == NUERAL_NETWORK) {
			BOOST_LOG_SEV(logger, NORMAL) << "Launching AI process...";
			aiThreads.push_back(std::make_shared<std::thread>(run_ai_script, serverSettings, name, playerNumber));
		}
    } else {
		BOOST_LOG_SEV(logger, ERROR) << "TODO: ai was false...";
        return;
    }
}

void Lobby::setPlayerAsSpectator(const std::string &playerName, bool isSpectator) {
    std::unique_lock<std::recursive_mutex> lock{mutex};

    if (isSpectator) {
        if (isPlayerSpectating(playerName)) {
			BOOST_LOG_SEV(logger, ERROR) << "Player is already spectating";
            return;
        }

        auto slot = getSlot(playerName);
        if (slot == nullptr) {
            return;
        }
        if (slot->getType() != HUMAN) {
			BOOST_LOG_SEV(logger, ERROR) << "Wasn't human";
            return;
        }

        auto connection = slot->getConnection();

        spectators.push_front(connection);
        slot->setType(EMPTY);
        slot->setConnection(nullptr);
    } else {
		BOOST_LOG_SEV(logger, ERROR) << "Implement ma"
								  "king a player no longer a spectator...";
    }
}

Lobby::Lobby(Logger& logger, ServerSettings& serverSettings, const std::string& name) :
	logger{logger},
	serverSettings{serverSettings},
    mutex{},
    name{name},
    options{},
    playerSlots{},
    spectators{} {
    for (int i = 0; i < INITIAL_NUM_SLOTS; i++) {
        playerSlots.push_back(std::make_shared<PlayerSlot>());
    }
}

PlayerSlot *Lobby::getSlot(const std::string &pName) {
    for (auto & slot : playerSlots) {
        if (slot->getName() == pName) {
            return slot.get();
        }
    }
	BOOST_LOG_SEV(logger, ERROR) << "Player not found: " << pName;
    return nullptr;
}

void Lobby::debug(JsonWriter &writer) {
    std::unique_lock<std::recursive_mutex> lock{mutex};
    writer.writeBeginObject();

    writeAddress(this, writer);
    writer.writeString("name", name);
    writer.writeKey("launch-options");
    options.debug(writer);
    writer.writeInt("number-of-slots", playerSlots.size());

    writer.writeBeginArray("slots");
    for (auto & slot : playerSlots) {
        slot->debug(writer);
    }
    writer.writeEndArray();

    writer.writeBeginArray("spectators");
    for (auto & spectator : spectators) {
        writer.writeString(spectator->name);
    }
    writer.writeEndArray();

    writer.writeEndObject();
}

bool Lobby::isPlayerSpectating(const std::string &name) {
	std::unique_lock<std::recursive_mutex> lock{mutex};
    for (auto & connection : spectators) {
        if (connection->name == name) {
            return true;
        }
    }
    return false;
}

void Lobby::updateClients() {
	std::unique_lock<std::recursive_mutex> lock{mutex};
    for (auto & slot : playerSlots) {
        auto connection = slot->getConnection();
        if (connection == nullptr) {
            continue;
        }
        sendFilledStatusUpdate(this, connection);
    }

    for (auto & spectator : spectators) {
        sendFilledStatusUpdate(this, spectator);
    }
}

void sendFilledStatusUpdate(Lobby *lobby, ConnectionContext *context) {
    std::unique_lock<std::recursive_mutex> lock{context->writeLock};
    context->writer.writeBeginObject();
    context->writer.writeInt("message-type", aod::common::message::LOBBY_STATUS_UPDATE);
    context->writer.writeBoolean("connected", true);
    context->writer.writeBoolean("ready", lobby->isPlayerReady(context->name));
    context->writer.writeBoolean("all-ready", lobby->areAllPlayersReady());
    context->writer.writeBoolean("spectating", lobby->isPlayerSpectating(context->name));
    context->writer.writeString("lobby-name", lobby->name);
    context->writer.writeEndObject();
    context->writer.flush();
}

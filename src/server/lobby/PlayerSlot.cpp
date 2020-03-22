//
// Created by thallock on 2/15/20.
//

#include "PlayerSlot.h"

#include <utility>

bool PlayerSlot::isReady() {
    return type != EMPTY && ready;
}

const std::string &PlayerSlot::getName() {
    if (!connectedPlayer) {
        return INVALID;
    }

    return connectedPlayer->name;
}

void PlayerSlot::setReady(bool ready) {
    this->ready = ready;
}

void PlayerSlot::setConnection(ConnectionContext *player) {
    this->connectedPlayer = player;
}

PlayerType PlayerSlot::getType() {
    return type;
}

void PlayerSlot::setType(PlayerType type) {
    this->type = type;
}

ConnectionContext *PlayerSlot::getConnection() {
    return connectedPlayer;
}

void PlayerSlot::debug(JsonWriter& writer) {
    writer.writeBeginObject();

    writeAddress(this, writer);

    switch (type) {
        case AI:
            writer.writeString("type", "ai");
            break;
        case HUMAN:
            writer.writeString("type", "human");
            break;
        case EMPTY:
            writer.writeString("type", "empty");
            break;
        default:
            writer.writeString("type", "error");
            break;
    }
    writer.writeBoolean("ready", ready);
    if (connectedPlayer == nullptr) {
        writer.writeNull("connection");
    } else {
        writer.writeString("connection", connectedPlayer->name);
    }
    writer.writeEndObject();
}

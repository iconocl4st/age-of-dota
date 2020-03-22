//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_PLAYERSLOT_H
#define PLAYING_PLAYERSLOT_H


#include "../../common/network/ConnectionContext.h"

enum PlayerType {
    EMPTY = 0,
    HUMAN = 1,
    AI = 2,
};

const std::string INVALID = "invalid";

class PlayerSlot : public JsonDebuggable {
private:
    PlayerType type;
//    std::shared_ptr<ConnectionContext> connectedPlayer;
    ConnectionContext *connectedPlayer;
    bool ready;

public:
    PlayerSlot() : type{EMPTY}, connectedPlayer{nullptr}, ready{false} {}

    PlayerType getType();
    bool isReady();
    void setReady(bool ready);
    void setConnection(ConnectionContext *player);

    const std::string& getName();

    void setType(PlayerType type);

    ConnectionContext *getConnection();

    void debug(JsonWriter& writer);
};


#endif //PLAYING_PLAYERSLOT_H

//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_PLAYERSLOT_H
#define PLAYING_PLAYERSLOT_H


#include <src/common/spec/player_types.h>
#include "../../common/network/ConnectionContext.h"


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

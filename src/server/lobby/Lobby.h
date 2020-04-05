//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_LOBBY_H
#define PLAYING_LOBBY_H

#include <string>
#include <list>
#include <vector>
#include <thread>
#include <src/common/spec/player_types.h>

#include "LaunchOptions.h"
#include "PlayerSlot.h"
#include "../../common/network/ConnectionContext.h"
#include "../../common/util/logging_levels.h"
#include "../app/ServerSettings.h"


// TODO: HACK WHILE WE USE RUN_BOTH
void setLobbyAiMethod(std::function<void(const std::string&, int)> aiLauncher);

class Lobby : public JsonDebuggable {
private:
	Logger& logger;
	ServerSettings& serverSettings;

public:
    std::recursive_mutex mutex;

    std::string name;
    LaunchOptions options;
    std::vector<std::shared_ptr<PlayerSlot>> playerSlots;
    std::list<ConnectionContext *> spectators;
    std::list<std::shared_ptr<std::thread>> aiThreads;

    PlayerSlot * getSlot(const std::string& name);
    bool isPlayerSpectating(const std::string& name);

public:
    Lobby(Logger& logger, ServerSettings& serverSettings, const std::string& name);

//    void kick(int);

    void updateClients();

    bool areAllPlayersReady();
    bool isPlayerReady(const std::string& playerName);
    void setPlayerReady(const std::string& playerName, bool ready);
    int getNumPlayers() { return playerSlots.size(); }

    void setPlayerAsSpectator(const std::string& playerName, bool isSpectator);
    void setNumPlayers(int numPlayers);
    void setPlayerAsAi(int playerNumber, bool isAi, AiType aiType);

    void addPlayer(ConnectionContext *context, PlayerType type, int playerNumber);

    void debug(JsonWriter& writer);
};


void sendFilledStatusUpdate(Lobby *lobby, ConnectionContext *context);


#endif //PLAYING_LOBBY_H

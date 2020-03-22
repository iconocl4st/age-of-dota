//
// Created by thallock on 3/7/20.
//

#ifndef PLAYING_SERVERGAMESTATE_H
#define PLAYING_SERVERGAMESTATE_H

#include <thread>
#include <condition_variable>

#include "../../common/network/ConnectionContext.h"
#include "../../common/state/CommonState.h"
#include "../lobby/LaunchOptions.h"
#include "IncomingRequests.h"

class ServerGameState : JsonDebuggable {
private:

public:
    std::string uuid;

    bool isTicking{false};

    std::map<int, ConnectionContext *> playerConnections;
    std::vector<ConnectionContext *> spectatorConnections;
    std::vector<std::shared_ptr<std::thread>> aiThreads;

    std::function<EntityId()> entityIdGenerator;

    // Some statistics

	aod::common::state::CommonState sharedState;

	aod::server::state::IncomingRequests incomingRequests;

	std::mutex tickCompleteMutex;
	std::condition_variable tickCompleteCv;
	std::set<std::string> tickCompleteConnections;
	bool firstTick{true};
public:
    ServerGameState(const std::string& uuid, LaunchOptions& options);
    ~ServerGameState();


    void debug(JsonWriter& writer);

    int getNumPlayers();

    EntityId generateEntityId();

	void broadcast(std::function<void(ConnectionContext&)> f);
	void broadcastToPlayers(std::function<void(ConnectionContext&)> f);
	void flushAll();

	int getPlayerNumber(const std::string& connectionName);

	void requestAiActions();
	void receivedAiReady(const std::string& connectionName);
	void waitUntilAllReady();
};


#endif //PLAYING_SERVERGAMESTATE_H

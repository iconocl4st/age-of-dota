//
// Created by thallock on 2/14/20.
//

#ifndef PLAYING_SERVERCONTEXT_H
#define PLAYING_SERVERCONTEXT_H

#include "../../common/util/ThreadPool.h"
#include "../../common/util/RandomGenerator.h"
#include "../sim/ServerGameState.h"
#include "../lobby/Lobby.h"
#include "../../common/util/logging_levels.h"
#include "ServerSettings.h"

namespace aod::server::app {

class ServerContext : public JsonDebuggable {
private:
	std::condition_variable_any closingCv;

	std::recursive_mutex connectionsMutex;
	std::list<ConnectionContext *> connections;

	std::recursive_mutex connectionNumberMutex;
	int currentConnectionNumber{0};

	int getAndIncrementCurrentConnectionNumber();

	void removeLobby(const std::string& lobby);

	std::shared_ptr<ServerGameState> createGame(LaunchOptions& options);

	bool accepting{true};

public:
	// move to private...
	ServerSettings& settings;
	ThreadPool connectionPool;
	RandomGenerator numberGenerator;

	std::recursive_mutex lobbiesMutex;
	std::list<std::shared_ptr<Lobby>> lobbies;

	std::recursive_mutex gamesMutex;
	std::list<std::shared_ptr<ServerGameState>> games;

	Logger logger;

	ServerContext(ServerSettings& settings);

	void createLobby(const std::string &basicString);
	std::shared_ptr<Lobby> findLobby(const std::string &lobbyName);
	void cancelLobby(Lobby *lobby);

	void launchGame(Lobby *lobby);
	std::shared_ptr<ServerGameState> getGame(const std::string &gameUuid);
	void removeGame(const std::string& uuid);
	void waitForGamesToEnd();

	void debug(JsonWriter &writer);

	void runServerSocketLoop(int port);
	void handleConnection(int socket, int currentSocketNo);

	bool isRunning() { return accepting; }
};


void runServer();

}


#endif //PLAYING_SERVERCONTEXT_H

//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_UICLIENTCONTEXT_H
#define PLAYING_UICLIENTCONTEXT_H

#include <lobbybrowser.h>
#include <gamewindow.h>
#include <thread>

#include "../ui/LobbyBrowserAdapter.h"
#include "../ui/GameWindowAdapterImpl.h"
#include "../../client_common/state/ClientGameState.h"
#include "../../common/network/ConnectionContext.h"
#include "../state/UiClientGameState.h"
#include "../../common/util/logging_levels.h"
#include "../../client_common/util/ClientSettings.h"


// TODO: rename namespace
namespace aod::client::app {

class UiClientContext {

public:
	Logger logger;
	aod::client::common::ClientSettings settings;

	aod::client::ui::LobbyBrowserAdapter browserHandler{*this};
	LobbyBrowser lobbyBrowser{nullptr, &browserHandler};

	aod::client::graphics::GameWindowAdapterImpl gameAdapter{*this};
	GameWindow gameWindow{nullptr, &gameAdapter};


	std::recursive_mutex gameMutex;

	boost::optional<std::function<void(UiClientContext&)>> onConnect;
	boost::optional<std::function<void(UiClientContext&)>> onReady;
	boost::optional<std::function<void(UiClientContext&)>> onCloseGame;
private:
	std::recursive_mutex connectionMutex;
	ConnectionContext *connectionContext{nullptr};

	UiClientGameState *clientGameState{nullptr};

	void renderLoop();
	std::thread *renderThread{nullptr};
public:

	void runConnection(const std::string& address, int port);
	ConnectionContext *getConnection();


	void launchGame(PlayerLaunchOptions& options);
	void closeGame();
	bool isInGame() { return clientGameState != nullptr; }
	UiClientGameState *getGameState() { if (!isInGame()) { throw std::runtime_error("Not in a game."); } return clientGameState; }
};

int runClient(int argc, char **argv,
	void (*onConnect)(UiClientContext& context),
	void (*onReady)(UiClientContext& context),
	void (*onClose)(UiClientContext& context)
);

}


#endif //PLAYING_UICLIENTCONTEXT_H

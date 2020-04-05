//
// Created by thallock on 3/6/20.
//

#include "run_spectator_client.h"

#include <QApplication>

#include "../../client_ui/app/UiClientContext.h"
#include "../../server/app/ServerContext.h"


namespace startup_tasks {

void onConnect(aod::client::app::UiClientContext &context) {
	std::string lobbyName{"New Lobby"};

	context.browserHandler.createLobby(lobbyName);
	context.browserHandler.connect(lobbyName);
	context.browserHandler.setSpectator(true);
	context.browserHandler.setNumPlayers(2);
	context.browserHandler.setPlayerAsAi(0, true, SCRIPT_AI);

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1s);

	context.browserHandler.setPlayerAsAi(1, true, SCRIPT_AI);
}

void onReady(aod::client::app::UiClientContext &context) {
	context.browserHandler.launch();
}

void onClose(aod::client::app::UiClientContext &context) {
	auto *application = (QApplication *) QApplication::instance();
	application->closeAllWindows();
	application->exit(0);
}

}

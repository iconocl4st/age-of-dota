//
// Created by thallock on 2/17/20.
//

#include <thread>

#include "../../server/app/ServerContext.h"
#include "../../client_ui/app/UiClientContext.h"
#include "run_spectator_client.h"
#include "../../client_script/run_script_ai.h"
#include "../../common/util/signal_handler.h"

#include "../../common/state/quad_grid/test_quad_grid.h"

int main(int argc, char **argv) {

	test_quad_grid();

//	register_signal_handler();
//
//	setLobbyAiMethod(&runScriptAi);
//
//    std::thread server(aod::server::app::runServer);
//
//    using namespace std::chrono_literals;
//    std::this_thread::sleep_for(1s); // TODO (async io)
//
//    int clientRet = aod::client::app::runClient(argc, argv,
//    	&startup_tasks::onConnect,
//        &startup_tasks::onReady,
//		&startup_tasks::onClose
//	);
//
//    server.join();
//
//    return clientRet;
}

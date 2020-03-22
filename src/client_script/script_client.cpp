//
// Created by thallock on 3/11/20.
//

#include "run_script_ai.h"
#include "../common/util/signal_handler.h"

int main(int argc, char **argv)  {
	register_signal_handler();

	std::string lobbyName{argv[1]};
	int playerNumber = std::atoi(argv[2]);

	return runScriptAi(lobbyName, playerNumber);
}
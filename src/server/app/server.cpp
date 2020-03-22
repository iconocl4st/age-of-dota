//
// Created by thallock on 2/16/20.
//

#include "ServerContext.h"
#include "../../common/util/signal_handler.h"

int main(int argc, char **argv) {
	register_signal_handler();
    aod::server::app::runServer();
    return 0;
}
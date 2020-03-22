//
// Created by thallock on 2/16/20.
//

#include "UiClientContext.h"
#include "../../common/util/signal_handler.h"

void doNothing(aod::client::app::UiClientContext&) {}

int main(int argc, char **argv) {
	register_signal_handler();
    return aod::client::app::runClient(argc, argv, &doNothing, &doNothing, &doNothing);
}
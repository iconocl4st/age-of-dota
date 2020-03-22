//
// Created by thallock on 3/6/20.
//

#ifndef PLAYING_RUN_SPECTATOR_CLIENT_H
#define PLAYING_RUN_SPECTATOR_CLIENT_H


#include "../../client_ui/app/UiClientContext.h"

namespace startup_tasks {

void onConnect(aod::client::app::UiClientContext&);
void onReady(aod::client::app::UiClientContext&);
void onClose(aod::client::app::UiClientContext&);


}


#endif //PLAYING_RUN_SPECTATOR_CLIENT_H

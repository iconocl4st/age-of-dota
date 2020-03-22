//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_SEND_NOOP_H
#define PLAYING_SEND_NOOP_H

#include "ConnectionContext.h"

void sendNoop(ConnectionContext& connection);

void sendCloseConnection(ConnectionContext& context);

#endif //PLAYING_SEND_NOOP_H

//
// Created by thallock on 2/17/20.
//

#ifndef PLAYING_SOCKETS_H
#define PLAYING_SOCKETS_H

#include "../util/ThreadPool.h"

void runServerSocketLoop(ThreadPool& pool, int port);

int createClientSocket(const std::string& address, int port);

#endif //PLAYING_SOCKETS_H

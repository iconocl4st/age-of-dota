//
// Created by thallock on 3/8/20.
//


// https://www.geeksforgeeks.org/socket-programming-cc/

#include "../../common/network/sockets.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cstdlib>

#include "../app/ServerContext.h"
#include "../../common/network/CloseOnExit.h"


void aod::server::app::ServerContext::runServerSocketLoop(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
		BOOST_LOG_SEV(logger, ERROR) << "socket failed";
        exit(EXIT_FAILURE);
    }
	CloseOnExit coe{server_fd};

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
		BOOST_LOG_SEV(logger, ERROR) << "setsockopt";
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
		BOOST_LOG_SEV(logger, ERROR) << "bind failed";
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
		BOOST_LOG_SEV(logger, ERROR) << "listen error";
        exit(EXIT_FAILURE);
    }

    while (accepting && currentConnectionNumber < 3) {
		int conNo = getAndIncrementCurrentConnectionNumber();

		BOOST_LOG_SEV(logger, DEBUG) << "Waiting for connection #" << conNo;

        new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
        if (new_socket < 0) {
			BOOST_LOG_SEV(logger, ERROR) << "accept error";
            exit(EXIT_FAILURE);
        }

		// TODO: Could make them shorter lived, at some point, maybe

        connectionPool.enqueue([this, new_socket, conNo]() { handleConnection(new_socket, conNo); });
    }

	waitForGamesToEnd();
    accepting = false;
}

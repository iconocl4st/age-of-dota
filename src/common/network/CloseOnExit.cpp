//
// Created by thallock on 3/11/20.
//

#include "CloseOnExit.h"

#include <unistd.h>

CloseOnExit::CloseOnExit(int fd) : fd{fd} {}

CloseOnExit::~CloseOnExit() {
	close(fd);
}

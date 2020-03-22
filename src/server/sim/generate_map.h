//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_GENERATE_MAP_H
#define PLAYING_GENERATE_MAP_H

#include "ServerGameState.h"
#include "../../common/util/RandomGenerator.h"

#include <random>

void randomly_generate_map(RandomGenerator& generator, ServerGameState *gameState);

#endif //PLAYING_GENERATE_MAP_H

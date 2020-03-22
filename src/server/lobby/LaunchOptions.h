//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_LAUNCHOPTIONS_H
#define PLAYING_LAUNCHOPTIONS_H


#include "../../common/json/JsonDebuggable.h"
#include "../../common/spec/GameSpecification.h"

#define WIDTH 50

class LaunchOptions : public JsonDebuggable {
public:
    void debug(JsonWriter& writer);

	std::shared_ptr<GameSpecification> createGameSpecification() {
    	return createGameSpec(WIDTH, 1200.0 / 1920.0 * WIDTH);
    }
};


#endif //PLAYING_LAUNCHOPTIONS_H

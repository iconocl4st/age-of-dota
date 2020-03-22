//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_INCOMINGREQUESTS_H
#define PLAYING_INCOMINGREQUESTS_H

#include <map>
#include <mutex>

#include "../../common/state/definitions.h"
#include "../../common/state/Actions.h"

namespace aod::server::state {

class IncomingRequests {
public:
	std::recursive_mutex mutex;
	std::map<EntityId, std::shared_ptr<aod::common::action::Action>> requestedActions;

	IncomingRequests() : mutex{}, requestedActions{} {
		std::unique_lock<std::recursive_mutex> lock{mutex};
	}


	void set(EntityId, std::shared_ptr<aod::common::action::Action>);
};

}


#endif //PLAYING_INCOMINGREQUESTS_H

//
// Created by thallock on 3/11/20.
//

#include "IncomingRequests.h"

void aod::server::state::IncomingRequests::set(EntityId entityId, std::shared_ptr<aod::common::action::Action> action) {
	std::unique_lock<std::recursive_mutex> lock{mutex};
	requestedActions[entityId] = action;
}

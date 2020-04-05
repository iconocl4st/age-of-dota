//
// Created by thallock on 4/5/20.
//

#ifndef AGE_OF_DOTA_NEARESTENTITYSEARCHRESULTS_H
#define AGE_OF_DOTA_NEARESTENTITYSEARCHRESULTS_H

#include <functional>

#include "../definitions.h"
#include "../Point.h"

namespace aod::common::state { class CommonState; }
class Entity;

class NearestEntityQuery {
public:
	Point location;
	aod::common::state::CommonState& state;
	std::function<bool(Entity&)> include;
};


class NearestEntityQueryResults {
public:
	EntityId entityId;
	double distance;
	bool intersects;
	bool foundEntity;
};


#endif //AGE_OF_DOTA_NEARESTENTITYSEARCHRESULTS_H

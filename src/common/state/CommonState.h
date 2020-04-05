//
// Created by thallock on 2/5/20.
//

#ifndef PLAYING_COMMONSTATE_H
#define PLAYING_COMMONSTATE_H

#include <set>
#include <map>
#include <mutex>
#include <boost/ptr_container/ptr_map.hpp>

#include "../spec/GameSpecification.h"
#include "Actions.h"
#include "../state/quad_tree/QuadTree.h"
#include "../json/JsonDebuggable.h"
#include "src/common/state/quad_grid/QuadGrid.h"
#include "ResourceMap.h"
#include "Point.h"
#include "../spec/WeaponFire.h"
#include "LocationManager.h"
#include "quad_tree2/QuadTree2.h"

class Entity;

namespace aod::common::state {

class CommonState : public JsonDebuggable {
private:
	std::recursive_mutex entityMutex{};
	std::map<EntityId, std::shared_ptr<std::recursive_mutex>> entityLocks{};

public:
	std::shared_ptr<GameSpecification> specification;

	GameTime currentTime{0};
	std::chrono::high_resolution_clock::time_point lastTickTime{std::chrono::high_resolution_clock::now()};

	std::set<EntityId> entityIds{};

	std::map<EntityId, ResourceMap> carrying;
	std::map<EntityId, std::shared_ptr<EntitySpec>> typeManager;
	std::map<EntityId, std::shared_ptr<action::Action>> actionManager;

	std::map<EntityId, Player> playerManager;
	std::map<EntityId, GameTime> ageManager;
	std::map<EntityId, double> healthManager;
	std::map<EntityId, double> maxHealthManager;
	std::map<EntityId, double> movementSpeedManager;
	std::map<EntityId, double> attackSpeedManager;
	std::map<EntityId, double> rotationSpeedManager;
	std::map<EntityId, double> accuracyManager;
	std::map<EntityId, double> lineOfSightManager;
	std::map<EntityId, double> depositSpeedManager;
	std::map<EntityId, double> collectSpeedManager;


	QuadTree2 locationManager;
	QuadGrid occupied;

	std::map<EntityId, double> orientationManager;
	std::map<EntityId, Movement> movements;

	std::map<EntityId, ResourceMap> totalResourcesCollected;

	std::map<EntityId, uint32_t> capacityManager;

	std::map<ProjectileId, WeaponFire> attacks;

	CommonState(std::shared_ptr<GameSpecification> spec);

	GameTime interpolateTime(std::chrono::high_resolution_clock::time_point t);

	void debug(JsonWriter& writer);

	EntityId initializeEntityLock(std::function<EntityId(void)> getEntityId);
	void removeEntity(EntityId entityId);

	void updateLocations();

	Point getBounds() { return Point{(double) specification->width, (double) specification->height}; }

	friend Entity;
};

}

#endif //PLAYING_COMMONSTATE_H

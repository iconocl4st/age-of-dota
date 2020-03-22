
//
// Created by thallock on 2/5/20.
//

#include "CommonState.h"

namespace aod::common::state {

CommonState::CommonState(std::shared_ptr<GameSpecification> spec) :
	specification{spec},
	occupied{2, spec->width, spec->height},
	locationManager{0, 0, (float) spec->width, (float) spec->height, 5} {

}

void CommonState::debug(JsonWriter &writer) {
	writer.writeBeginObject();
	writer.writeEndObject();
}

GameTime CommonState::interpolateTime(std::chrono::high_resolution_clock::time_point t) {
	return currentTime + specification->gameSpeed * (t - lastTickTime).count() / 1000000000.0;
}

EntityId CommonState::initializeEntityLock(std::function<EntityId(void)> getEntityId) {
	std::unique_lock<std::recursive_mutex> lock{entityMutex};
	EntityId entityId = getEntityId();
	entityLocks[entityId] = std::make_shared<std::recursive_mutex>();
	return entityId;
}

void CommonState::updateLocations() {
	for (auto & it : movements) {
		if (!it.second.isMoving()) {
			continue;
		}

		Point p = it.second.interpolatePosition(currentTime);
		double o = it.second.interpolateOrientation(currentTime);

		locationManager.move(it.first, p);
		orientationManager[it.first] = o;
	}
}

void CommonState::removeEntity(EntityId entityId) {
	std::unique_lock<std::recursive_mutex> entitiesLock{entityMutex};
	std::shared_ptr<std::recursive_mutex> mutex = entityLocks[entityId];
	if (!mutex) {
		return;
	}
	std::unique_lock<std::recursive_mutex> entityLock{*(mutex.get())};
	entityLocks.erase(entityId);

	entityIds.erase(entityId);
	carrying.erase(entityId);
	typeManager.erase(entityId);
	actionManager.erase(entityId);
	playerManager.erase(entityId);
	ageManager.erase(entityId);
	healthManager.erase(entityId);
	maxHealthManager.erase(entityId);
	movementSpeedManager.erase(entityId);
	attackSpeedManager.erase(entityId);
	rotationSpeedManager.erase(entityId);
	accuracyManager.erase(entityId);
	lineOfSightManager.erase(entityId);
	depositSpeedManager.erase(entityId);
	collectSpeedManager.erase(entityId);
	orientationManager.erase(entityId);
	movements.erase(entityId);
	capacityManager.erase(entityId);

	locationManager.remove(entityId);

//	QuadGrid occupied;
}


}
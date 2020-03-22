//
// Created by thallock on 3/9/20.
//

#include "state_manipulator.h"
#include "Entity.h"

#include <memory>

namespace aod::common::state {

EntityId create_entity(CommonState &state, EntityCreationParams &params) {
	EntityId entityId = state.initializeEntityLock(*params.getEntityId);
	Entity entity{entityId, state};
	if (!entity.isLocked) {
		// unable to create...
		throw std::runtime_error("Uh oh");
	}
	state.entityIds.insert(entityId);

	state.ageManager[entityId] = state.currentTime;
	state.playerManager[entityId] = params.player;
	state.typeManager[entityId] = params.spec;
	state.orientationManager[entityId] = params.orientation;
	state.locationManager.add(entityId, params.location);

	if (params.capacity) state.capacityManager[entityId] = params.capacity.value();
	if (params.accuracy) state.accuracyManager[entityId] = params.accuracy.value();
	if (params.collectSpeed) state.collectSpeedManager[entityId] = params.collectSpeed.value();
	if (params.depositSpeed) state.depositSpeedManager[entityId] = params.depositSpeed.value();
	if (params.attackSpeed) state.attackSpeedManager[entityId] = params.attackSpeed.value();
	if (params.lineOfSight) state.lineOfSightManager[entityId] = params.lineOfSight.value();
	if (params.movementSpeed) state.movementSpeedManager[entityId] = params.movementSpeed.value();
	if (params.rotationSpeed) state.rotationSpeedManager[entityId] = params.rotationSpeed.value();
	if (params.maxHealth) state.maxHealthManager[entityId] = params.maxHealth.value();
	if (params.maxHealth) state.healthManager[entityId] = params.maxHealth.value();

	aod::common::state::setAction(state, entityId, std::make_shared<action::Idle>());

	return entityId;
}


void destroy_entity(CommonState &state, EntityId entityId) {
	// might have to do some other things too, eventually...
	state.removeEntity(entityId);
}

DamageApplicationResult apply_damage(CommonState &state, EntityId entityId, DamageInstance& damageInstance) {
	Entity entity{entityId, state};
	if (!entity.isLocked) {
		return DamageApplicationResult{-1, false};
	}

	double oldHealth = entity.getHealth().value();
	double newHealth = oldHealth - damageInstance.damageAmount;
	if (newHealth < 0) {
		state.healthManager[entityId] = 0;
		destroy_entity(state, entityId);
		return DamageApplicationResult{0, true};
	} else {
		state.healthManager[entityId] = newHealth;
		return DamageApplicationResult{newHealth, false};
	}

}

void setAction(CommonState& state, EntityId entityId, std::shared_ptr<aod::common::action::Action> action) {
	Entity entity{entityId, state};
	if (!entity.isLocked) {
		return;
	}
	auto orientation = entity.getOrientation();
	if (!orientation) {
		return;
	}
	auto location = entity.getLocation();
	if (!location) {
		return;
	}

	state.actionManager[entityId] = action;

	if (action->stopsMovement()) {
		state.movements[entityId] = Movement{};
		auto it = state.movements.find(entityId);

		it->second.beginLocation = location.value();
		it->second.directionX = 0;
		it->second.directionY = 0;
		it->second.movementSpeed = 0.0;

		it->second.beginOrientation = orientation.value();
		it->second.rotationDirection = 0.0;
		it->second.rotationSpeed = 0.0;

		it->second.beginTime = state.currentTime;
	}
}


}
//
// Created by thallock on 3/9/20.
//

#include "Entity.h"

//return tryEm(
//        state.capacityManager,
//[](EntitySpec *s) -> boost::optional<uint32_t> { return s->capacity; }
//);



#define INTERACTION_DISTANCE 1.0






boost::optional<uint32_t> Entity::getCapacity() {
    auto it = state.capacityManager.find(entityId);
    if (it != state.capacityManager.end()) {
        return it->second;
    }
    return boost::optional<uint32_t>{};
}

boost::optional<int> Entity::getRemainingCapacity() {
    return 0;
}

boost::optional<int> Entity::getCapacity(ResourceId resource) {
    return 0;
}

boost::optional<double> Entity::getLineOfSight() {
    auto it = state.lineOfSightManager.find(entityId);
    if (it != state.lineOfSightManager.end()) {
        return it->second;
    }
	return boost::optional<double>{};
}

boost::optional<double> Entity::getMovementSpeed() {
    auto it = state.movementSpeedManager.find(entityId);
    if (it != state.movementSpeedManager.end()) {
        return it->second;
    }
	return boost::optional<double>{};
}


boost::optional<double> Entity::getRotationSpeed() {
	auto it = state.rotationSpeedManager.find(entityId);
	if (it != state.rotationSpeedManager.end()) {
		return it->second;
	}
	return boost::optional<double>{};
}

boost::optional<double> Entity::getDepositSpeed() {
    auto it = state.depositSpeedManager.find(entityId);
    if (it != state.depositSpeedManager.end()) {
        return it->second;
    }
	return boost::optional<double>{};
}

boost::optional<double> Entity::getCollectSpeed() {
    auto it = state.collectSpeedManager.find(entityId);
    if (it != state.collectSpeedManager.end()) {
        return it->second;
    }
	return boost::optional<double>{};
}

boost::optional<double> Entity::getAttackSpeed() {
    auto it = state.attackSpeedManager.find(entityId);
    if (it != state.attackSpeedManager.end()) {
        return it->second;
    }
	return boost::optional<double>{};
}

boost::optional<double> Entity::getHealth() {
    auto it = state.healthManager.find(entityId);
    if (it != state.healthManager.end()) {
        return it->second;
    }
    return boost::optional<double>{};
}

boost::optional<double> Entity::getMaxHealth() {
    auto it = state.maxHealthManager.find(entityId);
    if (it != state.maxHealthManager.end()) {
        return it->second;
    }
	return boost::optional<double>{};
}

boost::optional<double> Entity::getOrientation() {
	auto it = state.orientationManager.find(entityId);
	if (it == state.orientationManager.end()) {
		return boost::optional<double>{};
	}
	return it->second;
}

boost::optional<Point> Entity::getLocation() {
	return state.locationManager.get(entityId);
}

std::shared_ptr<EntitySpec> Entity::getType() {
    auto it = state.typeManager.find(entityId);
    if (it == state.typeManager.end()) {
        return nullptr;
    }
    return it->second;
}

void Entity::debug(JsonWriter &writer) {
	writer.writeBeginObject();

	writer.writeInt("entity-id", entityId);

	auto type = getType();
	writer.writeString("type", type->name);

	auto location = getLocation();
	if (location) {
		writer.writeBeginObject("location");
		writer.writeDouble("x", location.value().x);
		writer.writeDouble("y", location.value().y);
		writer.writeEndObject();
	}

	auto cap = getCapacity();
	if (cap) writer.writeInt("capacity", cap.value());

	auto los = getLineOfSight();
	if (los) writer.writeInt("line-of-sight", los.value());

	auto mov = getMovementSpeed();
	if (mov) writer.writeInt("movement-speed", mov.value());

	auto rot = getRotationSpeed();
	if (rot) writer.writeInt("rotation-speed", rot.value());

	auto dep = getDepositSpeed();
	if (dep) writer.writeInt("deposit-speed", dep.value());

	auto col = getCollectSpeed();
	if (col) writer.writeInt("collect-speed", col.value());

	auto att = getAttackSpeed();
	if (att) writer.writeInt("attack-speed", att.value());

	auto health = getHealth();
	if (health) writer.writeInt("health", health.value());

	auto mhealth = getMaxHealth();
	if (mhealth) writer.writeInt("max-health", mhealth.value());

	writer.writeEndObject();
}

boost::optional<double> Entity::getOrientation(GameTime time) {
	auto it = state.movements.find(entityId);
	if (it == state.movements.end()) {
		return getOrientation();
	}
	return boost::optional<double>{it->second.interpolateOrientation(time)};
}

boost::optional<Point> Entity::getLocation(GameTime time) {
	auto it = state.movements.find(entityId);
	if (it == state.movements.end()) {
		return getLocation();
	}
	return boost::optional<Point>{it->second.interpolatePosition(time)};
}

boost::optional<Player> Entity::getPlayer() {
	auto it = state.playerManager.find(entityId);
	if (it == state.playerManager.end()) {
		return boost::optional<Player>();
	}
	return boost::optional<Player>(it->second);
}

std::shared_ptr<Shape> Entity::getShape() {
	return state.typeManager[entityId]->occupies;
}

boost::optional<Point> Entity::getInteractionPoint() {
	auto orientationO = getOrientation();
	if (!orientationO) return boost::optional<Point>{};
	auto orientation = orientationO.value();

	auto locationO = getLocation();
	if (!locationO) return boost::optional<Point>{};
	auto location = locationO.value();

	return Point{
		location.x + INTERACTION_DISTANCE * cosf(orientation),
		location.y + INTERACTION_DISTANCE * sinf(orientation)
	};
}

boost::optional<Point> Entity::getInteractionPoint(GameTime currentTime) {
	auto orientationO = getOrientation(currentTime);
	if (!orientationO) return boost::optional<Point>{};
	auto orientation = orientationO.value();

	auto locationO = getLocation(currentTime);
	if (!locationO) return boost::optional<Point>{};
	auto location = locationO.value();

	return Point{
		location.x + INTERACTION_DISTANCE * cosf(orientation),
		location.y + INTERACTION_DISTANCE * sinf(orientation)
	};
}

std::shared_ptr<aod::common::action::Action> Entity::getAction() {
	auto it = state.actionManager.find(entityId);
	if (it == state.actionManager.end()) {
		return std::shared_ptr<aod::common::action::Action>{};
	}
	return it->second;
}



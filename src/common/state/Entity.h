//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_ENTITY_H
#define PLAYING_ENTITY_H


#include "CommonState.h"


class Entity : public JsonDebuggable {
private:
    EntityId entityId;
    aod::common::state::CommonState& state;
	std::shared_ptr<std::recursive_mutex> mutex;
public:
	bool isLocked;
public:
    Entity(EntityId entityId, aod::common::state::CommonState& state) :
        entityId{entityId},
        state{state}
	{
		std::unique_lock<std::recursive_mutex> lock{state.entityMutex};

		isLocked = false;
		mutex = state.entityLocks[entityId];
		if (!mutex) {
			return;
		}
		mutex->lock();
		isLocked = true;
    }


    ~Entity() {
		if (isLocked) {
			mutex->unlock();
		}
    }

    boost::optional<uint32_t> getCapacity();
    boost::optional<int> getRemainingCapacity();
    boost::optional<int> getCapacity(ResourceId resource);

    boost::optional<Player> getPlayer();
    boost::optional<double> getLineOfSight();
    boost::optional<double> getMovementSpeed();
    boost::optional<double> getRotationSpeed();
    boost::optional<double> getDepositSpeed();
    boost::optional<double> getCollectSpeed();
    boost::optional<double> getAttackSpeed();
    boost::optional<double> getHealth();
    boost::optional<double> getMaxHealth();
	boost::optional<double> getOrientation();
	boost::optional<double> getOrientation(GameTime currentTime);
    boost::optional<Point> getLocation();
	boost::optional<Point> getLocation(GameTime currentTime);
	std::shared_ptr<aod::common::action::Action> getAction();

	boost::optional<Point> getInteractionPoint(GameTime currentTime);
	boost::optional<Point> getInteractionPoint();

    void debug(JsonWriter& writer);

	std::shared_ptr<EntitySpec> getType();

	std::shared_ptr<Shape> getShape();
};


#endif //PLAYING_ENTITY_H

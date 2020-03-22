//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_STATE_MANIPULATOR_H
#define PLAYING_STATE_MANIPULATOR_H

#include "CommonState.h"
#include "../spec/DamageInstance.h"
#include "manipulation_classes.h"

namespace aod::common::state {

EntityId create_entity(CommonState &state, EntityCreationParams &params);
void destroy_entity(CommonState &state, EntityId entityId);

DamageApplicationResult apply_damage(
	CommonState &state,
	EntityId entityId,
	DamageInstance& damageInstance
);

void setAction(CommonState& state, EntityId entity, std::shared_ptr<aod::common::action::Action> action);

//void set_carrying(CommonState &state, EntityId entity, ResourceId resource, int amount);

//void update_location(CommonState &state, EntityId entity, Point location);


}

#endif //PLAYING_STATE_MANIPULATOR_H

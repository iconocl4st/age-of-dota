//
// Created by thallock on 3/12/20.
//


#include "simulate.h"
#include "../../common/state/Entity.h"
#include "../network/server_game_messages.h"


namespace aod::server::engine::simulate {

void attack(aod::common::action::Attack *action, EntitySimInfo& simInfo) {
	Entity entity{simInfo.entity, simInfo.gState->sharedState};
	if (!entity.isLocked) {
		return;
	}

	// TODO: add channel time
	// TODO: inaccuracy

	WeaponFire fire{
		simInfo.gState->generateEntityId(), // TODO
		simInfo.entity,
		simInfo.tickInfo.tickBegin,
		entity.getInteractionPoint(simInfo.tickInfo.tickBegin).value(),
		entity.getOrientation().value()
	};
	simInfo.gState->sharedState.attacks[fire.id] = fire;

	simInfo.gState->broadcast([&simInfo, &fire](ConnectionContext &c) -> void {
		aod::server::message::sendProjectileLaunch(&c, fire);
	});

	auto newAction = std::make_shared<aod::common::action::Idle>();
	aod::common::state::setAction(simInfo.gState->sharedState, simInfo.entity, newAction);
	simInfo.gState->broadcast([&simInfo, &newAction](ConnectionContext &c) -> void {
		aod::server::message::sendEntityChangedAction(&c, simInfo.entity, newAction);
	});
}

}


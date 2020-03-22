//
// Created by thallock on 3/12/20.
//

#include "RandomMovementAiGroup.h"
#include "ScriptClientContext.h"
#include "../client_common/network/client_messages.h"

void assignMovement(ScriptClientContext& cntxt, EntityId entityId) {
	std::list<Point> points;
	int numPoints = cntxt.random.nextInt(0, 3);
	for (int i = 0; i < numPoints; i++) {
		points.emplace_back(
			cntxt.random.nextDouble(0, cntxt.clientState.sharedState.specification->width),
			cntxt.random.nextDouble(0, cntxt.clientState.sharedState.specification->height)
		);
	}
	aod::client::common::network::requestAction(
		cntxt.connectionContext,
		entityId,
		std::make_shared<aod::common::action::Move>(points, cntxt.random.nextAngle())
	);
}

void RandomMovementAiGroup::removeEntity(EntityId) {

}

void RandomMovementAiGroup::addEntity(ScriptClientContext& cntxt, EntityId entityId) {
	assignMovement(cntxt, entityId);
}

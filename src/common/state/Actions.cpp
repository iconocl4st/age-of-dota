//
// Created by thallock on 3/19/20.
//

#include "Actions.h"
#include "../util/angles.h"

#include <cmath>

namespace aod::common::action {

void parseAction(
	const std::string& keyName,
	JsonReader& reader,
	std::shared_ptr<aod::common::action::Action>& action
) {
	reader.readBeginObject(keyName);
	auto actionType = (aod::common::action::ActionType) reader.readInt("action-type");
	switch (actionType) {
		case aod::common::action::IDLE:
			action = std::make_shared<aod::common::action::Idle>();
			break;
		case aod::common::action::MOVE:
			action = std::make_shared<aod::common::action::Move>();
			break;
		case aod::common::action::COLLECT:
			action = std::make_shared<aod::common::action::Collect>();
			break;
		case aod::common::action::DEPOSIT:
			action = std::make_shared<aod::common::action::Deposit>();
			break;
		case aod::common::action::ATTACK:
			action = std::make_shared<aod::common::action::Attack>();
			break;
		case aod::common::action::STRAFE:
			action = std::make_shared<aod::common::action::Strafe>();
			break;
		default:
			throw std::runtime_error("Unhandled action type");
	}
	action->parseBody(reader);
	reader.readEndObject();
}


void sendAction(const std::string& keyName, JsonWriter& writer, std::shared_ptr<aod::common::action::Action> action) {
	writer.writeBeginObject(keyName);
	writer.writeInt("action-type", action->getType());
	action->sendBody(writer);
	writer.writeEndObject();
}


void Move::normalizeAngle() {
	desiredOrientation = normalize_angle(desiredOrientation);
}

void Move::placeWithinBounds(double width, double height) {
	for (auto& p : path) {
		p.placeWithinBounds(width, height);
	}
}


}
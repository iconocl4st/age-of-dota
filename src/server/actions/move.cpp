//
// Created by thallock on 3/12/20.
//

#include <cmath>
#include "simulate.h"
#include "../../common/state/Entity.h"
#include "../../common/state/LocationManager.h"
#include "../network/server_game_messages.h"
#include "../../common/util/angles.h"

namespace aod::server::engine::simulate {

double MOVEMENT_PRECISION{1e-4};

struct SimulationProgress {
	GameTime currentTime;
	GameTime tickEnd;

	Point currentLocation;
	double currentOrientation;

	int currentIndex;
	bool rotating;
};

namespace progress_report {
	enum ProgressReport {
		STEP_COMPLETED,
		RAN_OUT_OF_TIME,
		ERROR
	};
}

static progress_report::ProgressReport rotateTo(
	SimulationProgress& progress,
	double orientation,
	double rotationSpeed
) {
	double angleDelta = getAngleDirection(progress.currentOrientation, orientation);
	double angleDistance = getDistanceBetweenAngles(progress.currentOrientation, orientation);
	double timeToRotate = angleDistance / rotationSpeed;
	if (progress.currentTime + timeToRotate <
			progress.tickEnd - MOVEMENT_PRECISION) {
		progress.currentTime += timeToRotate;
		progress.currentOrientation = orientation;
		return progress_report::STEP_COMPLETED;
	}
	double remainingTime = progress.tickEnd - progress.currentTime;
	progress.currentOrientation = normalize_angle(
		progress.currentOrientation +
			angleDelta * rotationSpeed * remainingTime
	);
	progress.currentTime = progress.tickEnd;
	return progress_report::RAN_OUT_OF_TIME;
}

static progress_report::ProgressReport moveTowards(
	SimulationProgress& progress,
	const Point& destination,
	double movementSpeed,

	double n,
	double dx,
	double dy
) {
	double timeToReachDestination = n / movementSpeed;
	if (progress.currentTime + timeToReachDestination <
			progress.tickEnd - MOVEMENT_PRECISION) {
		progress.currentTime += timeToReachDestination;
		progress.currentLocation = destination;
		return progress_report::STEP_COMPLETED;
	}

	double remainingTime = progress.tickEnd - progress.currentTime;
	progress.currentLocation.x += remainingTime * movementSpeed * dx;
	progress.currentLocation.y += remainingTime * movementSpeed * dy;

	progress.currentTime = progress.tickEnd;
	return progress_report::RAN_OUT_OF_TIME;
}

static void updateCurrentMovement(SimulationProgress& progress, Movement& movement) {
	movement.beginTime = progress.currentTime;
	movement.beginLocation = progress.currentLocation;
	movement.beginOrientation = progress.currentOrientation;
	movement.directionX = 0.0;
	movement.directionY = 0.0;
	movement.rotationDirection = 0.0;
}

void move(aod::common::action::Move *action, EntitySimInfo& simInfo) {
	Entity entity{simInfo.entity, simInfo.gState->sharedState};
	if (!entity.isLocked) {
		return;
	}

	double movementSpeed = entity.getMovementSpeed().value();
	double rotationSpeed = entity.getRotationSpeed().value();
	Point originalLocation = entity.getLocation().value();
	double originalOrientation = entity.getOrientation().value();

	SimulationProgress progress;
	progress.currentTime = simInfo.tickInfo.tickBegin;
	progress.tickEnd = simInfo.tickInfo.getTickEnd();

	progress.currentLocation = originalLocation;
	progress.currentOrientation = originalOrientation;

	progress.currentIndex = action->currentIndex;
	progress.rotating = action->rotating;

	bool movementChanged{false};
	Movement mvmnt;
	mvmnt.rotationSpeed = rotationSpeed;
	mvmnt.movementSpeed = movementSpeed;

	if (!action->sentFirstMovement) {
		movementChanged = true;
		action->sentFirstMovement = true;
	}

	bool actionCompleted{false};
	while (progress.currentTime < progress.tickEnd - MOVEMENT_PRECISION) {
		updateCurrentMovement(progress, mvmnt);

		if (progress.currentIndex >= action->path.size()) {
			mvmnt.rotationDirection = getAngleDirection(progress.currentOrientation, action->desiredOrientation);
			auto result = rotateTo(progress, action->desiredOrientation, rotationSpeed);
			if (result == progress_report::RAN_OUT_OF_TIME) {
				break;
			}
			if (result == progress_report::STEP_COMPLETED) {
				movementChanged = true;
				actionCompleted = true;

				mvmnt.beginOrientation = action->desiredOrientation;
				break;
			}
			throw std::runtime_error("Did not expect this yet.");
		}

		Point desiredLocation = action->path[action->currentIndex];

		double dx = desiredLocation.x - progress.currentLocation.x;
		double dy = desiredLocation.y - progress.currentLocation.y;
		double n = std::sqrt(dx * dx + dy * dy);

		if (n < MOVEMENT_PRECISION) {
			movementChanged = true;
			progress.rotating = true;
			++progress.currentIndex;
			continue;
		}

		dx /= n;
		dy /= n;

		double requiredOrientation = normalize_angle(std::atan2(dy, dx));
		if (progress.rotating) {
			mvmnt.rotationDirection = getAngleDirection(progress.currentOrientation, requiredOrientation);
			auto result = rotateTo(progress, requiredOrientation, rotationSpeed);
			if (result == progress_report::RAN_OUT_OF_TIME) {
				break;
			}
			if (result == progress_report::STEP_COMPLETED) {
				progress.rotating = false;
				movementChanged = true;
				continue;
			}
			throw std::runtime_error("Did not expect this yet.");
		}

		double dT = getDistanceBetweenAngles(progress.currentOrientation, requiredOrientation);
		if (dT > MOVEMENT_PRECISION) {
			return;
//			throw std::runtime_error("Was supposed to be traveling at a given angle...");
		}

		mvmnt.directionX = dx;
		mvmnt.directionY = dy;
		auto result = moveTowards(progress, desiredLocation, movementSpeed, n, dx, dy);
		if (result == progress_report::RAN_OUT_OF_TIME) {
			break;
		}
		if (result == progress_report::STEP_COMPLETED) {
			++progress.currentIndex;
			progress.rotating = true;
			movementChanged = true;
			continue;
		}
		throw std::runtime_error("Did not expect this yet.");
	}
	action->currentIndex = progress.currentIndex;
	action->rotating = progress.rotating;
	simInfo.gState->sharedState.locationManager.move(simInfo.entity, progress.currentLocation);
	simInfo.gState->sharedState.orientationManager[simInfo.entity]
	= progress.currentOrientation;

	if (movementChanged) {
		simInfo.gState->broadcast([&simInfo, &mvmnt](ConnectionContext& c) -> void {
			aod::server::message::sendChangeEntityMovement(
				&c, simInfo.entity, mvmnt
			);
		});
	}

	if (actionCompleted) {
		auto newAction = std::make_shared<aod::common::action::Idle>();
		aod::common::state::setAction(simInfo.gState->sharedState, simInfo.entity, newAction);
		simInfo.gState->broadcast([&simInfo, &newAction](ConnectionContext &c) -> void {
			aod::server::message::sendEntityChangedAction(&c, simInfo.entity, newAction);
		});
	}
}


}


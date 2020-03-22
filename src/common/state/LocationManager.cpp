//
// Created by thallock on 2/5/20.
//

#include <cmath>
#include "LocationManager.h"
#include "../util/angles.h"

void Movement::parse(JsonReader *reader) {
	reader->readBeginObject();

	reader->readKey("location-begin");
	beginLocation.parse(reader);

	directionX = reader->readDouble("direction-x");
	directionY = reader->readDouble("direction-y");
	movementSpeed = reader->readDouble("movement-speed");

	beginOrientation = reader->readDouble("orientation-begin");
	rotationDirection = reader->readDouble("rotation-direction");
	rotationSpeed = reader->readDouble("rotation-speed");

	beginTime = reader->readDouble("movement-begin");
	reader->readEndObject();
}

void Movement::send(JsonWriter *writer) const {
	writer->writeBeginObject();

	writer->writeKey("location-begin");
	beginLocation.send(writer);

	writer->writeDouble("direction-x", directionX);
	writer->writeDouble("direction-y", directionY);
	writer->writeDouble("movement-speed", movementSpeed);

	writer->writeDouble("orientation-begin", beginOrientation);
	writer->writeDouble("rotation-direction", rotationDirection);
	writer->writeDouble("rotation-speed", rotationSpeed);

	writer->writeDouble("movement-begin", beginTime);

	writer->writeEndObject();
}

Point Movement::interpolatePosition(GameTime currentTime) {
	return Point{
		beginLocation.x + (currentTime - beginTime) * movementSpeed * directionX,
		beginLocation.y + (currentTime - beginTime) * movementSpeed * directionY
	};
}
double Movement::interpolateOrientation(GameTime currentTime) {
	return beginOrientation + rotationDirection * (currentTime - beginTime) * rotationSpeed;
}

void Movement::verify() {
	if (!isMoving()) {
		return;
	}
	if (rotationDirection == 0) {
		if (directionX == 0 && directionY == 0) {
			throw 0;
		}
		double theta = normalize_angle(std::atan2(directionY, directionX));
		if (getDistanceBetweenAngles(theta, beginOrientation) > 1e-4) {
			throw 0;
		}
	} else {
		if (directionX != 0 || directionY != 0) {
			throw 0;
		}
	}
}

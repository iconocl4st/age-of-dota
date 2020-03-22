//
// Created by thallock on 2/5/20.
//

#ifndef PLAYING_LOCATIONMANAGER_H
#define PLAYING_LOCATIONMANAGER_H


#include "definitions.h"
#include "Point.h"

class Movement : public JsonSendable {
public:
	Point beginLocation;
	double directionX;
	double directionY;
	double movementSpeed;

    double beginOrientation;
    double rotationDirection;
    double rotationSpeed;

    GameTime beginTime;

	Point interpolatePosition(GameTime currentTime);
	double interpolateOrientation(GameTime currentTime);

	bool isMoving() const { return directionX != 0 || directionY != 0 || rotationDirection != 0; }

    void send(JsonWriter *writer) const;
    void parse(JsonReader *reader);


	void verify();
};


#endif //PLAYING_LOCATIONMANAGER_H

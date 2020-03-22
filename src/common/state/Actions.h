//
// Created by thallock on 3/7/20.
//

#ifndef PLAYING_ACTIONS_H
#define PLAYING_ACTIONS_H

#include <vector>

#include "definitions.h"
#include "Point.h"

namespace aod::common::action {

enum ActionType {
	IDLE = 0,
	MOVE = 1,
	COLLECT = 2,
	DEPOSIT = 3,
	ATTACK = 4,
	STRAFE = 5,
};


class Action {
public:
	virtual ~Action() = default;
	virtual ActionType getType() = 0;
	virtual void sendBody(JsonWriter& writer) = 0;
	virtual void parseBody(JsonReader& reader) = 0;

	// maybe not needed...
	virtual bool stopsMovement() = 0;
};

class Idle : public Action {
	ActionType getType() { return IDLE; }

	void sendBody(JsonWriter& writer) {};
	void parseBody(JsonReader& reader) {};
	bool stopsMovement() { return true; }
};

class Move : public Action {
public:
	std::vector<Point> path;
	double desiredOrientation;
	int currentIndex;
	bool rotating;
	bool sentFirstMovement;
	bool move{true};

	Move() :
		path{},
		desiredOrientation{0},
		currentIndex{0},
		rotating{true},
		sentFirstMovement{false} {}

	Move(const std::list<Point>& pathL, double desiredOrientation) :
		path{},
		desiredOrientation{desiredOrientation},
		currentIndex{0},
		rotating{true},
		sentFirstMovement{false}
	{
		for (auto & p : pathL) {
			path.push_back(p);
		}
	}

	void normalizeAngle();

	void placeWithinBounds(double width, double height);

	ActionType getType() { return MOVE; }

	bool stopsMovement() { return false; }

	void sendBody(JsonWriter& writer) {
		writer.writeDouble("desired-orientation", desiredOrientation);

		writer.writeBeginArray("path");
		for (auto &destination : path) {
			destination.send(&writer);
		}
		writer.writeEndArray();
	};

	void parseBody(JsonReader& reader) {
		desiredOrientation = reader.readDouble("desired-orientation");
		reader.readBeginArray("path");
		while (!reader.atArrayEnd(json_token::BEGIN_OBJECT)) {
			double x = reader.readDouble("x");
			double y = reader.readDouble("y");
			reader.readEndObject();
			path.emplace_back(x, y);
		}
	};
};

class Collect : public Action {
public:
	ResourceId resourceType;

	ActionType getType() { return COLLECT; }

	bool stopsMovement() { return true; }

	void sendBody(JsonWriter& writer) {
		writer.writeInt("resource", resourceType);
	};
	void parseBody(JsonReader& reader) {
		resourceType = (ResourceId) reader.readInt("resource");
	};
};

class Deposit : public Action {
public:
	ResourceId resourceType;

	ActionType getType() { return DEPOSIT; }

	bool stopsMovement() { return true; }

	void sendBody(JsonWriter& writer) {
		writer.writeInt("resource", resourceType);
	};
	void parseBody(JsonReader& reader) {
		resourceType = (ResourceId) reader.readInt("resource");
	};
};

class Attack : public Action {
public:

	ActionType getType() { return ATTACK; }

	bool stopsMovement() { return true; }

	void sendBody(JsonWriter& writer) {};

	void parseBody(JsonReader& reader) {};
};

class Strafe : public Action {
public:
	bool isLeft;

	ActionType getType() { return STRAFE; }

	bool stopsMovement() { return false; }

	void sendBody(JsonWriter& writer) {
		writer.writeBoolean("is-left", isLeft);
	};
	void parseBody(JsonReader& reader) {
		isLeft = reader.readBoolean("is-left");
	};
};


void parseAction(const std::string& keyName, JsonReader& reader,
				 std::shared_ptr<aod::common::action::Action>& action);

void sendAction(const std::string& keyName, JsonWriter& writer, std::shared_ptr<aod::common::action::Action> action);

}




#endif //PLAYING_ACTIONS_H

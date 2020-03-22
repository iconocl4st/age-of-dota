//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_MANIPULATION_CLASSES_H
#define PLAYING_MANIPULATION_CLASSES_H

#include "../spec/GameSpecification.h"

namespace aod::common::state {

class EntityCreationParams : public JsonSendable {
public:
	std::shared_ptr<EntitySpec> spec;

	Point location;
	double orientation;
	Player player;

	std::function<EntityId(void)> *getEntityId;


	boost::optional<uint32_t> capacity;
	boost::optional<double> accuracy;
	boost::optional<double> collectSpeed;
	boost::optional<double> depositSpeed;
	boost::optional<double> attackSpeed;
	boost::optional<double> lineOfSight;
	boost::optional<double> maxHealth;
	boost::optional<double> movementSpeed;
	boost::optional<double> rotationSpeed;


	// make a &
	void parse(JsonReader *reader, std::shared_ptr<GameSpecification> specification);

	void send(JsonWriter *writer) const;
};



class DamageApplicationResult : public JsonSendable {
public:
	double newHealth;
	bool unitDies;

	DamageApplicationResult(double h, bool d) : newHealth{h}, unitDies{d} {}
	DamageApplicationResult() : DamageApplicationResult(0, false) {}

	void send(JsonWriter *writer) const;
	void parse(JsonReader *reader);
};


}


#endif //PLAYING_MANIPULATION_CLASSES_H

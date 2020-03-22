//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_DAMAGEINSTANCE_H
#define PLAYING_DAMAGEINSTANCE_H


#include "../state/Point.h"
#include "damage_types.h"
#include "../state/definitions.h"

class DamageInstance : public JsonSendable {
public:
	EntityId from;
	Point origin;
	damage_types::DamageType damageType;
	double damageAmount;

	DamageInstance() :
		DamageInstance{0, Point{0, 0}, damage_types::PELLET, 0} {}

	DamageInstance(EntityId from, Point origin, damage_types::DamageType type, double amount) :
		from{from},
		origin{origin},
		damageType{type},
		damageAmount{amount} {}

	void send(JsonWriter *writer) const;
	void parse(JsonReader *writer);
};


#endif //PLAYING_DAMAGEINSTANCE_H

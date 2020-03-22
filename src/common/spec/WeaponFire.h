//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_WEAPONFIRE_H
#define PLAYING_WEAPONFIRE_H


#include <set>
#include "../state/definitions.h"
#include "../state/Point.h"
#include "damage_types.h"

class WeaponFire : public JsonSendable {
public:
	ProjectileId id;
	EntityId launcher;
	GameTime launchTime;
	Point origin;
	damage_types::DamageType damageType{damage_types::PELLET};
	int numProjectiles;
	double projectileSpeed;
	double spread;
	double range;
	double orientation;
	double damage;

	std::set<int> sunkProjectiles;

	GameTime lastRender;

	WeaponFire();
	WeaponFire(ProjectileId id, EntityId launcher, GameTime launchTime, Point origin, double orientation);

	double getOrientation(int index);
	bool isProjectileSunk(int index);

	void send(JsonWriter *writer) const;
	void parse(JsonReader *reader);
};


#endif //PLAYING_WEAPONFIRE_H

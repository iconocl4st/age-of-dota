//
// Created by thallock on 3/13/20.
//

#include "WeaponFire.h"

#include <cmath>

WeaponFire::WeaponFire() : WeaponFire{0, 0, -1, Point{0, 0}, 0} {}

WeaponFire::WeaponFire(ProjectileId id, EntityId launcher, GameTime launchTime, Point origin, double orientation) :
	id{id},
	launcher{launcher},
	launchTime{launchTime},
	origin{origin},
	orientation{orientation},
	numProjectiles{15},
	projectileSpeed{20},
	spread{M_PI / 32},
	range{15},
	damage{15},
	lastRender{launchTime} {}

void WeaponFire::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeInt("id", id);
	writer->writeInt("launcher", launcher);
	writer->writeDouble("launch-time", launchTime);
	writer->writeInt("num-projectiles", numProjectiles);
	writer->writeDouble("projectile-speed", projectileSpeed);
	writer->writeDouble("spread", spread);
	writer->writeDouble("range", range);
	writer->writeDouble("orientation", orientation);
	writer->writeKey("origin");
	origin.send(writer);
	writer->writeEndObject();
}

void WeaponFire::parse(JsonReader *reader) {
	reader->readBeginObject();
	id = reader->readInt("id");
	launcher = reader->readInt("launcher");
	launchTime = reader->readDouble("launch-time");
	numProjectiles = reader->readInt("num-projectiles");
	projectileSpeed = reader->readDouble("projectile-speed");
	spread = reader->readDouble("spread");
	range = reader->readDouble("range");
	orientation = reader->readDouble("orientation");
	reader->readKey("origin");
	origin.parse(reader);
	reader->readEndObject();

	lastRender = launchTime;

}

double WeaponFire::getOrientation(int index) {
	if (numProjectiles > 1) {
		return orientation - spread + 2 * spread * index / (double) (numProjectiles - 1);
	} else {
		return orientation;
	}
}

bool WeaponFire::isProjectileSunk(int index) {
	return sunkProjectiles.find(index) != sunkProjectiles.end();
}

//
// Created by thallock on 3/7/20.
//

#ifndef PLAYING_ENTITYSPEC_H
#define PLAYING_ENTITYSPEC_H

#include <boost/optional.hpp>
#include <set>


#include "../state/Shape.h"
#include "../state/ResourceMap.h"
#include "../util/RandomGenerator.h"

namespace entity_classes {

enum EntityClass {
	OCCUPIES,
	OWNED,
};

}

class EntitySpec {
public:
	std::string name;
	std::shared_ptr<Shape> occupies;
	ResourceMap carrying;
	std::set<entity_classes::EntityClass> classes;
	std::string graphics;

	boost::optional<uint32_t> capacity; // should be generated

	boost::optional<TruncatedGaussian> accuracy;
	boost::optional<TruncatedGaussian> collectSpeed;
	boost::optional<TruncatedGaussian> depositSpeed;
	boost::optional<TruncatedGaussian> attackSpeed;
	boost::optional<TruncatedGaussian> lineOfSight;
	boost::optional<TruncatedGaussian> maxHealth;
	boost::optional<TruncatedGaussian> movementSpeed;
	boost::optional<TruncatedGaussian> rotationSpeed;
};

#endif //PLAYING_ENTITYSPEC_H

//
// Created by thallock on 3/7/20.
//

#ifndef PLAYING_GAMESPECIFICATION_H
#define PLAYING_GAMESPECIFICATION_H

#include <vector>

#include <boost/optional.hpp>

#include "EntitySpec.h"

class ResourceSpec {
public:
    int weight;
    std::string name;
};


class GenerationSpec {

};

class CraftSpec {
    ResourceMap input;
    ResourceMap output;
};

class GameSpecification : public JsonSendable {
public:
    int width;
    int height;

    double gameSpeed;

    //  visibility

    std::vector<std::shared_ptr<ResourceSpec>> resources;
    std::vector<std::shared_ptr<EntitySpec>> entities;
    std::vector<std::shared_ptr<CraftSpec>> crafting;

	std::shared_ptr<EntitySpec> findByName(const std::string& name);

    void send(JsonWriter *writer) const;
};

std::shared_ptr<GameSpecification> createGameSpec(int width, int height);


#endif //PLAYING_GAMESPECIFICATION_H

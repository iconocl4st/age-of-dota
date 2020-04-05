//
// Created by thallock on 3/7/20.
//

#include "GameSpecification.h"

std::shared_ptr<GameSpecification> createGameSpec(int width, int height) {
    auto spec = std::make_shared<GameSpecification>();

    spec->width = width;
    spec->height = height;

    spec->gameSpeed = 1.0;

    {
        auto *r = new ResourceSpec;
        r->name = "food";
        r->weight = 100;
        spec->resources.push_back(std::shared_ptr<ResourceSpec>{r});
    }
    {
        auto *r = new ResourceSpec;
        r->name = "bullet";
        r->weight = 1;
        spec->resources.push_back(std::shared_ptr<ResourceSpec>{r});
    }
    {
        auto *e = new EntitySpec;
        e->name = "human";
        e->graphics = "human";
        e->occupies = std::make_shared<Circle>(0.5);
        e->capacity = 500;
		e->accuracy = TruncatedGaussian{1, 0.1, 0.1, 2};
        e->lineOfSight = TruncatedGaussian{5, 0.01, 3, 7};
        e->movementSpeed = TruncatedGaussian{1.5, 0.5, 0.5, 5};
        e->rotationSpeed = TruncatedGaussian{1, 0.5, 0.1, 5};
        e->depositSpeed = TruncatedGaussian{1, 0.2, 0.1, 5};
        e->collectSpeed = TruncatedGaussian{1, 0.2, 0.1, 5};
        e->attackSpeed = TruncatedGaussian{1, 0.2, 0.1, 5};
        e->maxHealth = TruncatedGaussian{100, 5, 50, 200};
// e->carrying
// e->classes

        spec->entities.push_back(std::shared_ptr<EntitySpec>{e});
    }

	{
		auto *e = new EntitySpec;
		e->name = "tree";
		e->graphics = "tree";
		e->occupies = std::make_shared<Circle>(0.5);
		e->capacity = 500;
		e->accuracy = TruncatedGaussian{};
		e->lineOfSight = TruncatedGaussian{};
		e->movementSpeed = TruncatedGaussian{};
		e->rotationSpeed = TruncatedGaussian{};
		e->depositSpeed = TruncatedGaussian{};
		e->collectSpeed = TruncatedGaussian{};
		e->attackSpeed = TruncatedGaussian{};
		e->maxHealth = TruncatedGaussian{10000};

		spec->entities.push_back(std::shared_ptr<EntitySpec>{e});
	}
    return spec;
}

std::shared_ptr<EntitySpec> GameSpecification::findByName(const std::string& name) {
    for (auto & spec : entities) {
        if (spec->name == name) {
            return spec;
        }
    }
    return nullptr;
}

void GameSpecification::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeInt("width", width);
	writer->writeInt("height", height);
	writer->writeEndObject();
}

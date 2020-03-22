//
// Created by thallock on 3/9/20.
//

#include "generate_map.h"
#include "../app/ServerContext.h"
#include "../../common/state/state_manipulator.h"
#include "../network/server_game_messages.h"
#include "../../common/state/Point.h"

#include <random>
#include <math.h>

class RandomGenerationSpec {
public:
    std::function<int()> numHumansGen;
    std::function<double()> xGen;
    std::function<double()> yGen;
    std::function<double()> orientationGen;
    RandomGenerator& generator;

    RandomGenerationSpec(RandomGenerator& engine, int width, int height) :
        numHumansGen{[&engine]() -> int { return engine.nextInt(10, 15); }},
		xGen{[&engine, width]() -> double { return engine.nextDouble(0, (double) width); }},
		yGen{[&engine, height]() -> double { return engine.nextDouble(0, (double) height); }},
		orientationGen{[&engine]() -> double { return engine.nextAngle(); }},
		generator{engine}
    {}

    Point generatePoint() {
        return Point{xGen(), yGen()};
    }
};

void genDouble(RandomGenerator& engine, boost::optional<double>& output, boost::optional<TruncatedGaussian>& dist) {
	if (dist) {
		output = engine.nextGaussian(dist.value());
	}
}

void randomly_generate_map(RandomGenerator& engine, ServerGameState *gameState) {
    std::function<EntityId()> genId = [gameState]() -> EntityId { return gameState->generateEntityId(); };

    RandomGenerationSpec generationSpec{
    	engine,
        gameState->sharedState.specification->width,
        gameState->sharedState.specification->height
    };

	aod::common::state::EntityCreationParams creationParams;
    creationParams.getEntityId = &genId;
    creationParams.spec = gameState->sharedState.specification->findByName("human");
    if (creationParams.spec == nullptr) {
        throw std::runtime_error("Unable to find human spec");
    }

    for (int p = 1; p < gameState->getNumPlayers(); p++) {
        creationParams.player = p;
        int numHumans = generationSpec.numHumansGen();
        for (int h = 0; h < numHumans; h++) {
            creationParams.orientation = generationSpec.orientationGen();
            creationParams.location = generationSpec.generatePoint();

            creationParams.capacity = creationParams.spec->capacity;
            genDouble(generationSpec.generator, creationParams.accuracy, creationParams.spec->accuracy);
			genDouble(generationSpec.generator, creationParams.collectSpeed, creationParams.spec->collectSpeed);
			genDouble(generationSpec.generator, creationParams.depositSpeed, creationParams.spec->depositSpeed);
			genDouble(generationSpec.generator, creationParams.attackSpeed, creationParams.spec->attackSpeed);
			genDouble(generationSpec.generator, creationParams.lineOfSight, creationParams.spec->lineOfSight);
			genDouble(generationSpec.generator, creationParams.maxHealth, creationParams.spec->maxHealth);
			genDouble(generationSpec.generator, creationParams.movementSpeed, creationParams.spec->movementSpeed);
			genDouble(generationSpec.generator, creationParams.rotationSpeed, creationParams.spec->rotationSpeed);


            EntityId createdId = aod::common::state::create_entity(gameState->sharedState, creationParams);
            gameState->broadcast([createdId, &creationParams](ConnectionContext& connection) -> void {
            	//TODO &
            	aod::server::message::sendCreateEntity(&connection, createdId, &creationParams);
			});
        }
    }

    gameState->flushAll();
}

//
// Created by thallock on 3/13/20.
//

#include <exception>
#include <fstream>

#include "test_quadtree.h"
#include "QuadTree2.h"
#include "../../util/RandomGenerator.h"

void addOneElement() {
	QuadTree2 quadTree2{0, 0, 1, 1, 5};
	std::cout << printJsonDebuggable(&quadTree2) << std::endl;
	quadTree2.validate();

	quadTree2.add(0, Point{0.2, 0.1});
	std::cout << printJsonDebuggable(&quadTree2) << std::endl;
	quadTree2.validate();

	quadTree2.remove(0);
	std::cout << printJsonDebuggable(&quadTree2) << std::endl;
	quadTree2.validate();
}

EntityId getRandomId(RandomGenerator& generator, std::map<EntityId, Point>& cLocations) {
	std::vector<EntityId> entities;
	for (auto & it : cLocations) {
		entities.push_back(it.first);
	}
	return entities[generator.nextInt(0, entities.size())];
}

void testABunchOfRandom() {
	std::ofstream debugFile;
	debugFile.open("test_quadtree_output.txt");

	int maxSize = 0;
	QuadTree2 quadTree{0, 0, 1, 1, 3};

	debugFile << "Beginning with empty quad tree" << std::endl;
	debugFile << printJsonDebuggable(&quadTree) << std::endl;

	RandomGenerator generator{1776};

	std::map<EntityId, Point> cLocations;

	for (int i = 0; i < 10000; i++) {
		debugFile << "Test iteration: " << i << std::endl;
		double c = generator.nextDouble(0, 1);

		if (maxSize < cLocations.size()) {
			maxSize = cLocations.size();
		}
		debugFile << "The max size: " << maxSize << std::endl;

		if (cLocations.size() != quadTree.size()) {
			throw std::runtime_error("");
		}

//		if (i == 11) {
//			std::cout << "stop me" << std::endl;
//		}

		if (c < 0.5 && !cLocations.empty()) {
			// move
			Point newLocation{generator.nextDouble(0, 1),  generator.nextDouble(0, 1)};
			EntityId entityId = getRandomId(generator, cLocations);
			debugFile << "Moving " << entityId << " to " << newLocation << std::endl;

			Point expectedLocation = cLocations[entityId];
			Point actualLocation = quadTree.getLocation(entityId);
			if (expectedLocation != actualLocation) {
				throw std::runtime_error("");
			}

//			quadTree.move(entityId, newLocation);
			quadTree.remove(entityId);
			debugFile << printJsonDebuggable(&quadTree) << std::endl;
			quadTree.add(entityId, newLocation);
			debugFile << printJsonDebuggable(&quadTree) << std::endl;
			quadTree.validate();

			cLocations[entityId] = newLocation;
		} else if (c < 0.8 && !cLocations.empty()) {
			// remove
			EntityId entityId = getRandomId(generator, cLocations);
			debugFile << "Removing " << entityId << std::endl;

			Point expectedLocation = cLocations[entityId];
			Point actualLocation = quadTree.getLocation(entityId);
			if (expectedLocation != actualLocation) {
				throw std::runtime_error("");
			}

			quadTree.remove(entityId);
			debugFile << printJsonDebuggable(&quadTree) << std::endl;
			quadTree.validate();

			cLocations.erase(entityId);
		} else {
			// add
			Point newLocation{generator.nextDouble(0, 1),  generator.nextDouble(0, 1)};
			EntityId entityId;

			do {
				entityId = generator.nextInt(0, 10000000);
			} while (cLocations.find(entityId) != cLocations.end());

			cLocations[entityId] = newLocation;
			debugFile << "Creating " << entityId << " at " << newLocation << std::endl;

			quadTree.add(entityId, newLocation);
			debugFile << printJsonDebuggable(&quadTree) << std::endl;
			quadTree.validate();
		}
	}
}

void testQuadTree() {
//	addOneElement();
	testABunchOfRandom();
}
//
// Created by thallock on 3/13/20.
//

#include "QuadTreeLeafNode2.h"


QuadTreeLeafNode2::QuadTreeLeafNode2(double xmin, double ymin, double xmax, double ymax) :
	QuadTreeNode2{xmin, ymin, xmax, ymax, true} {}

void QuadTreeLeafNode2::debug(JsonWriter &writer)  {
	writer.writeBeginObject();
	writer.writeString("type", "leaf");

	writeSize(writer);
	writeBounds(writer);

	writer.writeBeginArray("entities");
	for (auto & id : entities) {
		writer.writeInt(id);
	}
	writer.writeEndArray();

	writer.writeEndObject();
}

EntityId QuadTreeLeafNode2::getValue() const {
	if (entities.size() != 1) {
		throw std::runtime_error("Should only be one");
	}
	return *entities.begin();
}

void QuadTreeLeafNode2::validate(int depth, int maxDepth, std::map<EntityId, Point>& locations) {
	if (depth > 1 && entities.empty()) {
		throw std::runtime_error("validation error");
	}

	for (auto & expectedIt : entities) {
		auto actualIt = locations.find(expectedIt);
		if (!contains(actualIt->second)) {
			throw std::runtime_error("");
		}
	}
}

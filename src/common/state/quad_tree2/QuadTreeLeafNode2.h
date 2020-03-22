//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_QUADTREELEAFNODE2_H
#define PLAYING_QUADTREELEAFNODE2_H

#include <set>

#include "QuadTreeNode2.h"

class QuadTreeLeafNode2 : public QuadTreeNode2 {
	std::set<EntityId> entities;

public:
	QuadTreeLeafNode2(double xmin, double ymin, double xmax, double ymax);

	EntityId getValue() const;

	void insert(EntityId id, bool inc) { entities.insert(id); if (inc) ++size; };
	void remove(EntityId id, bool dec) { entities.erase(id); if (dec) --size; }

	void debug(JsonWriter& writer);

	void validate(int depth, int maxDepth, std::map<EntityId, Point>& locations);
	const std::set<EntityId>& getEntities() const { return entities; };
};


#endif //PLAYING_QUADTREELEAFNODE2_H

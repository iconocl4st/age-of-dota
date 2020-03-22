//
// Created by thallock on 3/13/20.
//

#include "QuadTree2.h"
#include "QuadTreeBranchNode2.h"
#include "QuadTreeLeafNode2.h"

#include "../Entity.h"


QuadTree2::QuadTree2(double xmin, double ymin, double xmax, double ymax, int maxDepth) :
	root{new QuadTreeLeafNode2{xmin, ymin, xmax, ymax}},
	mutex{},
	locations{},
	maxDepth{maxDepth} {}

QuadTree2::~QuadTree2() {
	delete root;
}

static qt_branch getBranch(bool isLeft, bool isBottom) {
	if (isLeft) {
		if (isBottom) {
			return LOWER_LEFT;
		} else {
			return UPPER_LEFT;
		}
	} else {
		if (isBottom) {
			return LOWER_RIGHT;
		} else {
			return UPPER_RIGHT;
		}
	}
}

QuadTreeBranchNode2 *QuadTree2::subdivide(QuadTreeBranchNode2 *parent, qt_branch parentBranch, QuadTreeLeafNode2 *current) {
	if (current->getSize() != 1) {
		throw std::runtime_error("Requires having one element...");
	}
	EntityId toReInsert = current->getValue();
	Point location = locations[toReInsert];

	bool isLeft = location.x < current->getMidX();
	bool isBottom = location.y < current->getMidY();
	qt_branch currentBranch = getBranch(isLeft, isBottom);

	QuadTreeBranchNode2 *branchNode = new QuadTreeBranchNode2{
		current->getMinX(), current->getMinY(), current->getMaxX(), current->getMaxY(),
		currentBranch, current
	};

	if (parent == nullptr) {
		root = branchNode;
	} else {
		parent->set(parentBranch, branchNode);
	}

	return branchNode;
}

void QuadTree2::add(EntityId id, const Point& location) {
	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
	locations[id] = location;

	QuadTreeBranchNode2 *parent = nullptr;
	qt_branch currentBranch{LOWER_LEFT};
	QuadTreeNode2 *current = root;

	int depth = 1;
	while (depth < maxDepth && current->getSize() > 0) {
		if (current->isLeafNode) {
			current = subdivide(parent, currentBranch, (QuadTreeLeafNode2 *)current);
		}
		current->incrementSize();

		bool isLeft = location.x < current->getMidX(); // =?
		bool isBottom = location.y < current->getMidY();
		qt_branch branch = getBranch(isLeft, isBottom);

		auto *branchNode = (QuadTreeBranchNode2 *) current;
		parent = branchNode;
		currentBranch = branch;
		current = branchNode->getOrCreate(branch);
		++depth;
	}
	((QuadTreeLeafNode2 *) current)->insert(id, true);
}

static EntityId get_the_one(QuadTreeNode2 *node) {
	while (true) {
		if (node->getSize() != 1) {
			throw std::runtime_error("laskdjf;alksdjf");
		}
		if (node->isLeafNode) { break; }

		auto *branchNode = (QuadTreeBranchNode2 *) node;
		node = branchNode->get(LOWER_LEFT); if (node != nullptr) continue;
		node = branchNode->get(LOWER_RIGHT); if (node != nullptr) continue;
		node = branchNode->get(UPPER_LEFT); if (node != nullptr) continue;
		node = branchNode->get(UPPER_RIGHT); if (node != nullptr) continue;

		throw std::runtime_error("Should have had at least one child.");
	}

	return ((QuadTreeLeafNode2 *) node)->getValue();
}

void QuadTree2::remove(EntityId id) {
	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};

	auto it = locations.find(id);
	if (it == locations.end()) {
		throw std::runtime_error("No such id");
	}
	Point location = it->second;
	locations.erase(it);

	QuadTreeBranchNode2 *delParent{nullptr};
	qt_branch delBranch{LOWER_LEFT};
	QuadTreeNode2 *del{nullptr};
	int delSize{-1};

	QuadTreeBranchNode2 *parent = nullptr;
	QuadTreeNode2 *current = root;
	qt_branch parentBranch{LOWER_LEFT};
	while (!current->isLeafNode) {
		current->decrementSize();

		if (del == nullptr && current->getSize() <= 1) {
			delParent = parent;
			delBranch = parentBranch;
			del = current;
			delSize = current->getSize();
		}

		bool isLeft = location.x < current->getMidX();
		bool isBottom = location.y < current->getMidY();
		parentBranch = getBranch(isLeft, isBottom);

		parent = (QuadTreeBranchNode2 *) current;
		current = parent->get(parentBranch);
		if (current == nullptr) {
			throw std::runtime_error("Should not have been null");
		}
	}

	QuadTreeLeafNode2 *leaf = (QuadTreeLeafNode2 *) current;
	leaf->remove(id, true);

	if (leaf == root)  {
		return;
	}
	if (del == nullptr) {
		if (leaf->getSize() == 0) {
			parent->set(parentBranch, nullptr);
			delete leaf;
		}
		return;
	}

	parent->set(parentBranch, nullptr);

	if (delSize == 0) {
		if (leaf->getSize() > 0) {
			throw std::runtime_error("sldkjfslkdjf");
		}
	} else if (delSize == 1) {
		if (leaf->getSize() == 0) {
			EntityId other = get_the_one(del);
			leaf->insert(other, true);
		}
	} else {
		throw std::runtime_error("error");
	}
	if (delParent == nullptr) {
		leaf->setBounds(root->getMinX(), root->getMinY(), root->getMaxX(), root->getMaxY());
		root = leaf;
	} else {
		delParent->set(delBranch, leaf);
	}
	delete del;
}

void QuadTree2::move(EntityId id, const Point& newLocation) {
	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
	// TODO:
	remove(id);
	add(id, newLocation);
}

void QuadTree2::debug(JsonWriter &writer) {
	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
	writer.writeBeginObject();
	writer.writeBeginArray("locations");
	for (auto & it : locations) {
		writer.writeBeginObject();
		std::ostringstream ss;
		ss << it.first;
		writer.writeKey(ss.str());
		it.second.send(&writer);
		writer.writeEndObject();
	}
	writer.writeEndArray();

	root->debug(writer);

	writer.writeBeginObject();
}

void QuadTree2::visit(const Point& min, const Point& max, std::function<void(QuadTreeNode2 *)> visitor) {
	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};

	std::list<QuadTreeNode2 *> toVisit;
	toVisit.push_back(root);

	while (!toVisit.empty()) {
		QuadTreeNode2 *node = toVisit.back();
		toVisit.pop_back();

		if (!node->intersects(min, max)) {
			continue;
		}

		visitor(node);

		if (node->isLeafNode) { continue; }

		auto *branch = (QuadTreeBranchNode2 *) node;
		QuadTreeNode2 *child;
		child = branch->get(LOWER_LEFT);
		if (child != nullptr) toVisit.push_back(child);
		child = branch->get(LOWER_RIGHT);
		if (child != nullptr) toVisit.push_back(child);
		child = branch->get(UPPER_LEFT);
		if (child != nullptr) toVisit.push_back(child);
		child = branch->get(UPPER_RIGHT);
		if (child != nullptr) toVisit.push_back(child);
	}
}

void QuadTree2::validate() {
	root->validate(1, maxDepth, locations);
}

Point QuadTree2::getLocation(EntityId entityId) {
	return locations[entityId];
}

int QuadTree2::size() {
	return root->getSize();
}

void QuadTree2::visitLeaves(const Point &min, const Point &max,
							std::function<void(const Point &min, const Point &max)> visitor) {
	visit(min, max, [&visitor, &max, &min, this](QuadTreeNode2 *n) -> void {
		if (n->isLeafNode) {
			visitor(n->getMin(), n->getMax());
		}
	});
}

void
QuadTree2::visitEntities(aod::common::state::CommonState& state, const Point &min, const Point &max, std::function<void(EntityId, const Point &p)> visitor) {
	visit(min, max, [&state, &visitor, &max, &min, this](QuadTreeNode2 *n) -> void {
		if (!n->isLeafNode) {
			return;
		}

		auto *leaf = (QuadTreeLeafNode2 *) n;
		for (auto &entityId : leaf->getEntities()) {
			Point p = locations[entityId];
			Entity entity{entityId, state};

			if (!entity.isLocked) continue;
			auto shape = entity.getShape();
			double orientation = entity.getOrientation().value();
			if (shape->intersectsRectangle(p, orientation, min, max)) {
				visitor(entityId, p);
			}
		}
	});
}

//void QuadTree2::visit(const Point &min, const Point &max, std::function<void(EntityId, const Point &)> visitor) {
//	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
//	root->visit(min, max, visitor);
//}
//
//void QuadTree2::visitLeaves(std::function<void(const Point &, const Point &)> visitor) {
//
//}

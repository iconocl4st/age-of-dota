//
// Created by thallock on 2/6/20.
//

#include "QuadGrid.h"
#include "QuadGridLeafNode.h"
#include "QuadGridBranchNode.h"
#include "QuadGridFillerNode.h"
#include "branch_direction.h"

#include <list>

QuadGrid::QuadGrid(int numTypes, int maxX, int maxY, int defaultType) :
	root{new QuadGridLeafNode{0, 0, maxX, maxY, defaultType}},
	numTypes{numTypes},
	defaultType{defaultType} {}

QuadGrid::~QuadGrid() {
	delete root;
}

struct TreeIterInfo {
	QuadGridBranchNode *parent;
	branch_direction direction;
	QuadGridNode *child;

	TreeIterInfo(QuadGridBranchNode *parent, branch_direction direction, QuadGridNode *child) :
		parent{parent},
		direction{direction},
		child{child} {}
};

void QuadGrid::set(int xmin, int ymin, int xmax, int ymax, int type) {
	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};

	std::list<TreeIterInfo> nodes;
	nodes.emplace_back(nullptr, NONE, root);

	while (!nodes.empty()) {
		TreeIterInfo node_info = nodes.front();
		nodes.pop_front();

		if (node_info.child->getNodeType() == LEAF) {
			auto child = (QuadGridLeafNode *) node_info.child;
			if (child->containedIn(xmin, ymin, xmax, ymax)) {
				child->setType(type);
				continue;
			}

			QuadGridBranchNode *newChild = child->divide();
			if (node_info.parent == nullptr) {
				root = newChild;
			} else {
				node_info.parent->setChild(node_info.direction, newChild);
			}
			delete node_info.child;
			node_info.child = newChild;
		}

		if (node_info.child->getNodeType() == FILLER) {
			continue;
		}

		auto branch = (QuadGridBranchNode *) node_info.child;
		for (auto & direction : getAllDirections()) {
			auto *child = branch->getChild(direction);
			if (child->intersects(xmin, ymin, xmax, ymax)) {
				nodes.emplace_front(branch, direction, child);
			}
		}
	}
}

int QuadGrid::findNearest(int x, int y, Direction direction) {
	return 0;
}

int QuadGrid::getType(int x, int y) {
	QuadGridNode *node = root;
	while (node->getNodeType() == BRANCH) {
		auto *branch = (QuadGridBranchNode *) node;
		for (branch_direction d : getAllDirections()) {
			auto *child = branch->getChild(d);
			if (child->contains(x, y)) {
				node = child;
				break;
			}
			throw std::runtime_error("Could not find a child");
		}
	}
	if (node->getNodeType() == FILLER) {
		throw std::runtime_error("Descended into a filler node.");
	}
	auto *leaf = (QuadGridLeafNode *) node;
	return leaf->getType();
}

//
// Created by thallock on 4/5/20.
//

#include <stdexcept>
#include "QuadGridBranchNode.h"

QuadGridBranchNode::QuadGridBranchNode(
	int minX, int minY, int maxX, int maxY,
	QuadGridNode *ul, QuadGridNode *ur,
	QuadGridNode *ll, QuadGridNode *lr) :
	QuadGridNode{minX, minY, maxX, maxY, BRANCH},
	ul{ul},
	ur{ur},
	ll{ll},
	lr{lr} {}

QuadGridBranchNode::~QuadGridBranchNode() {
	delete ul;
	delete ur;
	delete ll;
	delete lr;
}

void QuadGridBranchNode::setChild(branch_direction direction, QuadGridNode *newChild) {
	switch (direction) {
		case NONE:
			throw std::runtime_error{"Unable to set direction for none."};
		case UPPER_LEFT:
			ul = newChild;
			break;
		case UPPER_RIGHT:
			ur = newChild;
			break;
		case LOWER_LEFT:
			ll = newChild;
			break;
		case LOWER_RIGHT:
			lr = newChild;
			break;
	}
}

QuadGridNode *QuadGridBranchNode::getChild(branch_direction direction) {
	switch (direction) {
		case NONE:
			throw std::runtime_error{"Unable to set direction for none."};
		case UPPER_LEFT:
			return ul;
		case UPPER_RIGHT:
			return ur;
		case LOWER_LEFT:
			return ll;
		case LOWER_RIGHT:
			return lr;
	}
}




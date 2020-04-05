//
// Created by thallock on 4/5/20.
//

#include "QuadGridLeafNode.h"
#include "QuadGridBranchNode.h"
#include "QuadGridFillerNode.h"

QuadGridLeafNode::QuadGridLeafNode(int minX, int minY, int maxX, int maxY, int type) :
	QuadGridNode{minX, minY, maxX, maxY, LEAF},
	type{type} {}


static QuadGridNode *createGridNode(int minX, int minY, int maxX, int maxY, int type) {
	if (minX == maxX || minY == maxY) {
		return new QuadGridFillerNode{minX, minY, maxX, maxY};
	} else {
		return new QuadGridLeafNode{minX, minY, maxX, maxY, type};
	}
}

QuadGridBranchNode *QuadGridLeafNode::divide() {
	int midX = (minX + maxX) / 2;
	int midY = (minY + maxY) / 2;

	auto *ll = createGridNode(minX, minY, midX, midY, type);
	auto *lr = createGridNode(midX, minY, maxX, midY, type);
	auto *ul = createGridNode(minX, midY, midX, maxY, type);
	auto *ur = createGridNode(midX, midY, maxX, maxY, type);

	return new QuadGridBranchNode{
		minX, minY, maxX, maxY,
		ul,	ur, ll, lr
	};
}


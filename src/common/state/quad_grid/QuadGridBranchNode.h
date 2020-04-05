//
// Created by thallock on 4/5/20.
//

#ifndef AGE_OF_DOTA_QUADGRIDBRANCHNODE_H
#define AGE_OF_DOTA_QUADGRIDBRANCHNODE_H


#include "QuadGridNode.h"
#include "branch_direction.h"

class QuadGridBranchNode : public QuadGridNode {
private:
	QuadGridNode *ul;
	QuadGridNode *ur;
	QuadGridNode *ll;
	QuadGridNode *lr;

public:
	QuadGridBranchNode(
		int minX, int minY, int maxX, int maxY,
		QuadGridNode *ul,
		QuadGridNode *ur,
		QuadGridNode *ll,
		QuadGridNode *lr
	);

	~QuadGridBranchNode();

	void setChild(branch_direction direction, QuadGridNode *newChild);
	QuadGridNode *getChild(branch_direction direction);
};


#endif //AGE_OF_DOTA_QUADGRIDBRANCHNODE_H

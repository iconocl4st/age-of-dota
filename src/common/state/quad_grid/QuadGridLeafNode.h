//
// Created by thallock on 4/5/20.
//

#ifndef AGE_OF_DOTA_QUADGRIDLEAFNODE_H
#define AGE_OF_DOTA_QUADGRIDLEAFNODE_H


#include "QuadGridNode.h"
#include "QuadGridBranchNode.h"

class QuadGridLeafNode : public QuadGridNode {
private:
	int type;
public:
	QuadGridLeafNode(int minX, int minY, int maxX, int maxY, int type);
	~QuadGridLeafNode() = default;

	inline void setType(int nType) { type = nType; }

	inline int getType() { return type; }

	QuadGridBranchNode *divide();
};


#endif //AGE_OF_DOTA_QUADGRIDLEAFNODE_H

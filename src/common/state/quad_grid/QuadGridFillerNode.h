//
// Created by thallock on 4/5/20.
//

#ifndef AGE_OF_DOTA_QUADGRIDFILLERNODE_H
#define AGE_OF_DOTA_QUADGRIDFILLERNODE_H

#include "QuadGridNode.h"

class QuadGridFillerNode : public QuadGridNode {
public:
	QuadGridFillerNode(int minX, int minY, int maxX, int maxY);
	~QuadGridFillerNode() = default;
};


#endif //AGE_OF_DOTA_QUADGRIDFILLERNODE_H

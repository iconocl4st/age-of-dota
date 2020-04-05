//
// Created by thallock on 4/5/20.
//

#ifndef AGE_OF_DOTA_QUADGRIDNODE_H
#define AGE_OF_DOTA_QUADGRIDNODE_H


enum NodeType {
	LEAF,
	BRANCH,
	FILLER,
};

class QuadGridNode {
protected:
	int minX;
	int minY;
	int maxX;
	int maxY;
	NodeType nodeType;

public:
	QuadGridNode(int minX, int minY, int maxX, int maxY, NodeType nType) :
		minX{minX},
		minY{minY},
		maxX{maxX},
		maxY{maxY},
		nodeType{nType} {}

	virtual ~QuadGridNode() = default;

	inline bool intersects(int minX2, int minY2, int maxX2, int maxY2) {
		return !(minX >= maxX2 || minY >= maxY2 || maxX <= minX2 || maxY <= minY2);
	}

	inline bool isExactly(int minX2, int minY2, int maxX2, int maxY2) {
		return minX == minX2 && minY == minY2 && maxX == maxX2 && maxY == maxY2;
	}

	inline bool containedIn(int minX2, int minY2, int maxX2, int maxY2) {
		return minX >= minX2 && maxX <= maxX2 && minY >= minY2 && maxY <= maxY2;
	}

	inline bool contains(int minX2, int minY2, int maxX2, int maxY2) {
		return minX2 >= minX && maxX2 <= maxX && minY2 >= minY && maxY2 <= maxY;
	}

	inline bool contains(int x, int y) {
		return x >= minX && x < maxX && y >= minY && y < maxY;
	}

	inline NodeType getNodeType() { return nodeType; }
};


#endif //AGE_OF_DOTA_QUADGRIDNODE_H

//
// Created by thallock on 2/6/20.
//

#include "QuadGrid.h"

class QuadGridNode {
public:
    int xmin, ymin;
    int xmax, ymax;
    int type;
};

class QuadGridLeafNode : public QuadGridNode {

};


class QuadGridBranchNode : public QuadGridNode {

};

class QuadGridFillerNode : public QuadGridNode {

};

QuadGrid::QuadGrid(int numTypes, int maxX, int maxY) {

}

QuadGrid::~QuadGrid() {

}

void QuadGrid::set(int xmin, int ymin, int xmax, int ymax, int type) {

}

int QuadGrid::findNearest(int x, int y, Direction direction) {
    return 0;
}

//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_QUADGRID_H
#define PLAYING_QUADGRID_H


#include "QuadGridNode.h"

#include <mutex>

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
};

class QuadGrid {
private:
    QuadGridNode *root;
    int numTypes;
	int defaultType;

	std::recursive_mutex mutex;

public:
    QuadGrid(int numTypes, int maxX, int maxY, int defaultType);
    ~QuadGrid();

    void set(int xmin, int ymin, int xmax, int ymax, int type);
    int getType(int x, int y); // probably only used for debugging...

    int findNearest(int x, int y, Direction direction);

};


#endif //PLAYING_QUADGRID_H

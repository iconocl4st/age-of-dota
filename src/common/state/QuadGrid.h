//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_QUADGRID_H
#define PLAYING_QUADGRID_H

class QuadGridNode;

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
};

class QuadGrid {
    QuadGridNode *root;
    int numTypes;

public:
    QuadGrid(int numTypes, int maxX, int maxY);
    ~QuadGrid();

    void set(int xmin, int ymin, int xmax, int ymax, int type);
    int findNearest(int x, int y, Direction direction);

};


#endif //PLAYING_QUADGRID_H

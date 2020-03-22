//
// Created by thallock on 2/5/20.
//

#ifndef PLAYING_QUADTREE_H
#define PLAYING_QUADTREE_H

#include <iostream>
#include <functional>
#include <boost/optional.hpp>

bool is_within(double xmin, double ymin, double xmax, double ymax, double x, double y);

// TODO: move to util somewhere
void writeDepth(std::ostream &ostream, int depth);

template <typename T> class QuadTreeNode;
template <typename T> class QuadTreeLeafNode;
template <typename T> class QuadTreeBranchNode;
template <typename T> class QuadTree;

#define BOUNDS_UL xmin, ymid, xmid, ymax
#define BOUNDS_UR xmid, ymid, xmax, ymax
#define BOUNDS_LL xmin, ymin, xmid, ymid
#define BOUNDS_LR xmid, ymin, xmax, ymid

template<typename T>
QuadTreeNode<T> *createBranchNode(
	double xmin, double ymin, double xmax, double ymax,
        QuadTreeLeafNode<T> *current,
        T t, double x, double y
);

template<typename T>
QuadTreeNode<T> *assignNode(
        bool isFirst, bool isSecond,
		double xmin, double ymin, double xmax, double ymax,
        QuadTreeLeafNode<T> *current,
        T t, double x, double y
);

#include "QuadTreeImpl.h"


#endif //PLAYING_QUADTREE_H

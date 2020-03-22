//
// Created by thallock on 3/10/20.
//

#ifndef PLAYING_QUADTREEIMPL_H
#define PLAYING_QUADTREEIMPL_H

#include <mutex>

#include "QuadTreeBranchNode.h"
#include "QuadTreeLeafNode.h"

template <typename T>
class QuadTree {
    QuadTreeNode<T> *root;
	std::recursive_mutex mutex;
public:
    QuadTree(double xmin, double ymin, double xmax, double ymax) :
		root{new QuadTreeLeafNode<T>{boost::optional<T>{}, xmin, ymin, xmax, ymax, -1, -1}} {
    }

    ~QuadTree() {
        delete root;
    }

    void add(T t, double x, double y)  {
    	std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
        root = root->add(t, x, y);
    }
    void remove(T t, double x, double y) {
		std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
        root = root->remove(t, x, y);
    }
    void move(T t, double xOld, double yOld, double xNew, double yNew) {
		std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
    	// TODO:
		root = root->remove(t, xOld, yOld);
		root = root->add(t, xNew, yNew);
//    	root = root->move(t, xOld, yOld, xNew, yNew);
    }

    void write(std::ostream &out) {
		std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
        root->write(out, 0);
    }

	void visit(double xmin, double ymin, double xmax, double ymax, std::function<void(T, double, double)> visitor) {
		std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
		root->visit(xmin, ymin, xmax, ymax, visitor);
    }

    void visitLeaves(std::function<void(double, double, double, double)> visitor) {
		std::unique_lock<std::recursive_mutex> uniqueLock{mutex};
		root->visitLeaves(visitor);
    }
};


#endif //PLAYING_QUADTREEIMPL_H

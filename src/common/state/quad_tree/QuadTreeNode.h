//
// Created by thallock on 3/10/20.
//

#ifndef PLAYING_QUADTREENODE_H
#define PLAYING_QUADTREENODE_H


#include "QuadTree.h"

template<typename T>
class QuadTreeNode {
public:

	double xmin, ymin;
	double xmax, ymax;

    QuadTreeNode(double xmin, double ymin, double xmax, double ymax) :
            xmin{xmin},
            ymin{ymin},
            xmax{xmax},
            ymax{ymax} {}

    virtual ~QuadTreeNode() {};

    virtual QuadTreeNode<T> *add(T t, double x, double y) = 0;
    virtual QuadTreeNode<T> *remove(T t, double x, double y) = 0;
	virtual QuadTreeNode<T> *move(T t, double xOld, double yOld, double xNew, double yNew) = 0;
	virtual void visit(double xmin, double ymin, double xmax, double ymax, std::function<void(T, double, double)> visitor) = 0;

    virtual bool isEmpty() = 0;

    bool contains(double x, double y) {
        return is_within(xmin, ymin, xmax, ymax, x, y);
    }

    bool intersects(double xmin, double ymin, double xmax, double ymax) {
		return !(
			QuadTreeNode<T>::xmin > xmax ||
			QuadTreeNode<T>::xmax < xmin ||
			QuadTreeNode<T>::ymin > ymax ||
			QuadTreeNode<T>::ymax < ymin
		);
	}

    void setBounds(double xmin, double ymin, double xmax, double ymax) {
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
    }

    void write(std::ostream &ostream, int depth) {
        writeDepth(ostream, depth);
        ostream << "[" << xmin << "," << xmax << "],[" << ymin << "," << ymax << "] ";
        writeInternals(ostream, depth);
    }

    virtual void writeInternals(std::ostream &ostream, int depth) = 0;

    virtual void visitLeaves(std::function<void(double, double, double, double)> visitor) = 0;
};

#endif //PLAYING_QUADTREENODE_H

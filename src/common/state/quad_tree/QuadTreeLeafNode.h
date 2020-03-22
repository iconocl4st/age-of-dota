//
// Created by thallock on 3/10/20.
//

#ifndef PLAYING_QUADTREELEAFNODE_H
#define PLAYING_QUADTREELEAFNODE_H

#include <boost/optional.hpp>

#include "QuadTreeNode.h"
// #include "QuadTree.h"

template <typename T>
class QuadTreeLeafNode : public QuadTreeNode<T> {
public:
    boost::optional<T> t;
    double x, y;

    QuadTreeLeafNode(boost::optional<T> t, double xmin, double ymin, double xmax, double ymax, double x, double y) :
            QuadTreeNode<T>{xmin, ymin, xmax, ymax},
            x{x}, y{y},
            t{t} {}

    ~QuadTreeLeafNode() {}

    void writeInternals(std::ostream &ostream, int depth) {
        ostream << "Leaf node: " << x << "," << y << ": ";
        if (!t) {
            ostream << "empty";
        } else {
            ostream << "implement this";
        }
        ostream << '\n';
    };

    QuadTreeNode<T> *add(T t, double x, double y) {
        if (!QuadTreeNode<T>::contains(x, y)) {
            return this;
        }

        if (!this->t) {
            this->t = t;
            this->x = x;
            this->y = y;
            return this;
        }

        return createBranchNode(
                QuadTreeNode<T>::xmin,
                QuadTreeNode<T>::ymin,
                QuadTreeNode<T>::xmax,
                QuadTreeNode<T>::ymax,
                this, t, x, y
        );
    }

    QuadTreeNode<T> *remove(T t, double x, double y) {
        if (!QuadTreeNode<T>::contains(x, y)) {
            return this;
        }
        if (this->t == t) {
            this->t = boost::optional<T>{};
            this->x = -1;
            this->y = -1;
            return this;
        }
        throw 0;
    }

    QuadTreeNode<T> *move(T t, double xOld, double yOld, double xNew, double yNew) {
        bool containsOld = QuadTreeNode<T>::contains(xOld, yOld);
        bool containsNew = QuadTreeNode<T>::contains(xNew, yNew);
        if (containsOld && !containsNew) {
            return remove(t, xOld, yOld);
        }
        if (!containsOld && containsNew) {
            return add(t, xOld, yOld);
        }
        if (!containsOld && !containsNew) {
            return this;
        }
        if (!this->t || this->t != t) {
        	throw 0;
        }
        x = xNew;
        y = yNew;
        return this;
    }

	void visit(double xmin, double ymin, double xmax, double ymax, std::function<void(T, double, double)> visitor) {
		if (!QuadTreeNode<T>::intersects(xmin, ymin, xmax, ymax)) {
			return;
		}
		if (!t) {
			return;
		}
		visitor(t.value(), x, y);
	}

    bool isEmpty()  {
        return !t;
    }


	void visitLeaves(std::function<void(double, double, double, double)> visitor) {
		visitor(
			QuadTreeNode<T>::xmin,
			QuadTreeNode<T>::ymin,
			QuadTreeNode<T>::xmax,
			QuadTreeNode<T>::ymax
		);
    }
};

template <typename T>
QuadTreeNode<T> *assignNode(
        bool isFirst, bool isSecond,
        double xmin, double ymin, double xmax, double ymax,
        QuadTreeLeafNode<T> *current,
        T t, double x, double y
) {
    if (isFirst) {
        if (isSecond) {
            return createBranchNode(xmin, ymin, xmax, ymax, current, t, x, y);
        } else {
            current->setBounds(xmin, ymin, xmax, ymax);
            return current;
        }
    } else if (isSecond) {
        return new QuadTreeLeafNode<T>{boost::optional<T>{t}, xmin, ymin, xmax, ymax, x, y};
    }
    return new QuadTreeLeafNode<T>{boost::optional<T>{}, xmin, ymin, xmax, ymax, -1, -1};
}


#endif //PLAYING_QUADTREELEAFNODE_H

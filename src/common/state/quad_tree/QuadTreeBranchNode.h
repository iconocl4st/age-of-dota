//
// Created by thallock on 3/10/20.
//

#ifndef PLAYING_QUADTREEBRANCHNODE_H
#define PLAYING_QUADTREEBRANCHNODE_H

#include "QuadTreeNode.h"

template<typename T>
class QuadTreeBranchNode : public QuadTreeNode<T> {
public:
    QuadTreeNode<T> *upperLeft;
    QuadTreeNode<T> *upperRight;
    QuadTreeNode<T> *lowerLeft;
    QuadTreeNode<T> *lowerRight;
    int size;

    QuadTreeBranchNode(
        double xmin, double ymin, double xmax, double ymax,
        QuadTreeNode<T> *upperLeft,
        QuadTreeNode<T> *upperRight,
        QuadTreeNode<T> *lowerLeft,
        QuadTreeNode<T> *lowerRight,
        int size
    ) :
            QuadTreeNode<T>{xmin, ymin, xmax, ymax},
            upperLeft{upperLeft},
            upperRight{upperRight},
            lowerLeft{lowerLeft},
            lowerRight{lowerRight},
            size{size} {}

    ~QuadTreeBranchNode() {
        delete upperLeft;
        delete upperRight;
        delete lowerLeft;
        delete lowerRight;
    }

    void writeInternals(std::ostream &ostream, int depth) {
        ostream << "Branch node:\n";
        upperLeft->write(ostream, depth + 1);
        upperRight->write(ostream, depth + 1);
        lowerLeft->write(ostream, depth + 1);
        lowerRight->write(ostream, depth + 1);
    };

    bool isEmpty() {
        return upperLeft->isEmpty() && upperRight->isEmpty() && lowerLeft->isEmpty() && lowerRight->isEmpty();
    }

    QuadTreeNode<T> *add(T t, double x, double y) {
        if (!QuadTreeNode<T>::contains(x, y)) {
            return this;
        }
        upperLeft = upperLeft->add(t, x, y);
        upperRight = upperRight->add(t, x, y);
        lowerLeft = lowerLeft->add(t, x, y);
        lowerRight = lowerRight->add(t, x, y);
        ++size;

        return this;
    }
    QuadTreeNode<T> *remove(T t, double x, double y) {
        if (!QuadTreeNode<T>::contains(x, y)) {
            return this;
        }

        upperLeft = upperLeft->remove(t, x, y);
        upperRight = upperRight->remove(t, x, y);
        lowerLeft = lowerLeft->remove(t, x, y);
        lowerRight = lowerRight->remove(t, x, y);

        --size;

        if (size == 1) {
			QuadTreeLeafNode<T> *singleLeafNode = new QuadTreeLeafNode<T>{
				boost::optional<T>{},
				QuadTreeNode<T>::xmin,
				QuadTreeNode<T>::ymin,
				QuadTreeNode<T>::xmax,
				QuadTreeNode<T>::ymax,
				-1, -1
			};

			QuadTreeLeafNode<T> *o;
			if ((o = (QuadTreeLeafNode<T> *) upperLeft)->t)  {
				singleLeafNode->t = o->t.value();
				singleLeafNode->x = o->x;
				singleLeafNode->y = o->y;
			} else if ((o = (QuadTreeLeafNode<T> *) upperRight)->t)  {
				singleLeafNode->t = o->t.value();
				singleLeafNode->x = o->x;
				singleLeafNode->y = o->y;
			} else if ((o = (QuadTreeLeafNode<T> *) lowerLeft)->t)  {
				singleLeafNode->t = o->t.value();
				singleLeafNode->x = o->x;
				singleLeafNode->y = o->y;
			} else if ((o = (QuadTreeLeafNode<T> *) lowerRight)->t)  {
				singleLeafNode->t = o->t.value();
				singleLeafNode->x = o->x;
				singleLeafNode->y = o->y;
			} else {
				throw 0;
			}

			delete this;
			return singleLeafNode;
        }

        if (size == 1) {
            QuadTreeLeafNode<T> *emptyLeafNode = new QuadTreeLeafNode<T>{
                boost::optional<T>{},
                QuadTreeNode<T>::xmin,
                QuadTreeNode<T>::ymin,
                QuadTreeNode<T>::xmax,
                QuadTreeNode<T>::ymax,
                -1, -1
            };
            delete this;
            return emptyLeafNode;
        }
        return this;
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

        upperLeft = upperLeft->move(t, xOld, yOld, xNew, yNew);
        upperRight = upperRight->move(t, xOld, yOld, xNew, yNew);
        lowerLeft = lowerLeft->move(t, xOld, yOld, xNew, yNew);
        lowerRight = lowerRight->move(t, xOld, yOld, xNew, yNew);
        return this;
    }
	void visit(double xmin, double ymin, double xmax, double ymax, std::function<void(T, double, double)> visitor) {
		if (!QuadTreeNode<T>::intersects(xmin, ymin, xmax, ymax)) {
			return;
		}
		upperLeft->visit(xmin, ymin, xmax, ymax, visitor);
		upperRight->visit(xmin, ymin, xmax, ymax, visitor);
		lowerLeft->visit(xmin, ymin, xmax, ymax, visitor);
		lowerRight->visit(xmin, ymin, xmax, ymax, visitor);
    }

	void visitLeaves(std::function<void(double, double, double, double)> visitor) {
    	upperLeft->visitLeaves(visitor);
		upperRight->visitLeaves(visitor);
		lowerLeft->visitLeaves(visitor);
		lowerRight->visitLeaves(visitor);
    }
};

template <typename T>
QuadTreeNode<T> *createBranchNode(
    double xmin, double ymin, double xmax, double ymax,
        QuadTreeLeafNode<T> *current,
        T t, double x, double y
) {
    float xmid = (xmin + xmax) / 2;
    float ymid = (ymin + ymax) / 2;

    bool ful = is_within(BOUNDS_UL, current->x, current->y);
    bool fur = is_within(BOUNDS_UR, current->x, current->y);
    bool fll = is_within(BOUNDS_LL, current->x, current->y);
    bool flr = is_within(BOUNDS_LR, current->x, current->y);

    bool sul = is_within(BOUNDS_UL, x, y);
    bool sur = is_within(BOUNDS_UR, x, y);
    bool sll = is_within(BOUNDS_LL, x, y);
    bool slr = is_within(BOUNDS_LR, x, y);

    return new QuadTreeBranchNode<T>{
            xmin, ymin, xmax, ymax,
            assignNode(ful, sul, BOUNDS_UL, current, t, x, y),
            assignNode(fur, sur, BOUNDS_UR, current, t, x, y),
            assignNode(fll, sll, BOUNDS_LL, current, t, x, y),
            assignNode(flr, slr, BOUNDS_LR, current, t, x, y),
            2
    };
}


#endif //PLAYING_QUADTREEBRANCHNODE_H

//
// Created by thallock on 2/5/20.
//

#include "QuadTree.h"





bool is_within(double xmin, double ymin, double xmax, double ymax, double x, double y) {
    return xmin <= x && ymin <= y && xmax > x && ymax > y;
}

void writeDepth(std::ostream &ostream, int depth) {
    for (int i = 0; i < depth; i++) {
        ostream << '\t';
    }
}

























































//
//
//
//class QuadTreeLeafNode;
//class QuadTreeBranchNode;
//
//
//static bool is_within(float xmin, float ymin, float xmax, float ymax, float x, float y) {
//    return xmin <= x && ymin <= y && xmax > x && ymax > y;
//}
//
//static void writeDepth(std::ostream &ostream, int depth) {
//    for (int i = 0; i < depth; i++) {
//        ostream << '\t';
//    }
//}
//
//static QuadTreeNode *createBranchNode(
//        float xmin, float ymin, float xmax, float ymax,
//        QuadTreeLeafNode *current,
//        void *ptr, float x, float y
//);
//
//class QuadTreeNode {
//public:
//
//    float xmin, ymin;
//    float xmax, ymax;
//
//    QuadTreeNode(float xmin, float ymin, float xmax, float ymax) :
//            xmin{xmin},
//            ymin{ymin},
//            xmax{xmax},
//            ymax{ymax} {}
//
//    virtual ~QuadTreeNode() {};
//
//    virtual QuadTreeNode *add(void *ptr, float x, float y) = 0;
//    virtual QuadTreeNode *remove(void *ptr, float x, float y) = 0;
//    virtual bool isEmpty() = 0;
//
//    bool contains(float x, float y) {
//        return is_within(xmin, ymin, xmax, ymax, x, y);
//    }
//
//    void setBounds(float xmin, float ymin, float xmax, float ymax) {
//        this->xmin = xmin;
//        this->ymin = ymin;
//        this->xmax = xmax;
//        this->ymax = ymax;
//    }
//
//    void write(std::ostream &ostream, int depth) {
//        writeDepth(ostream, depth);
//        ostream << "[" << xmin << "," << xmax << "],[" << ymin << "," << ymax << "] ";
//        writeInternals(ostream, depth);
//    }
//
//    virtual void writeInternals(std::ostream &ostream, int depth) = 0;
//};
//
//class QuadTreeLeafNode : public QuadTreeNode {
//public:
//    void *ptr;
//    float x, y;
//
//    QuadTreeLeafNode(void *ptr, float xmin, float ymin, float xmax, float ymax, float x, float y) :
//            QuadTreeNode{xmin, ymin, xmax, ymax},
//            x{x}, y{y},
//            ptr{ptr} {}
//
//    ~QuadTreeLeafNode() {}
//
//    void writeInternals(std::ostream &ostream, int depth) {
//        ostream << "Leaf node: " << x << "," << y << ": " << ptr << '\n';
//    };
//
//    QuadTreeNode *add(void *ptr, float x, float y) {
//        if (!contains(x, y)) {
//            return this;
//        }
//
//        if (this->ptr == nullptr) {
//            this->ptr = ptr;
//            this->x = x;
//            this->y = y;
//            return this;
//        }
//
//        return createBranchNode(xmin, ymin, xmax, ymax, this, ptr, x, y);
//    }
//
//    QuadTreeNode *remove(void *ptr, float x, float y) {
//        if (!contains(x, y)) {
//            return this;
//        }
//        if (this->ptr == ptr) {
//            this->ptr = nullptr;
//            this->x = -1;
//            this->y = -1;
//            return this;
//        }
//        throw 0;
//    }
//
//    bool isEmpty()  {
//        return ptr == nullptr;
//    }
//};
//
//class QuadTreeBranchNode : public QuadTreeNode {
//public:
//    QuadTreeNode *upperLeft;
//    QuadTreeNode *upperRight;
//    QuadTreeNode *lowerLeft;
//    QuadTreeNode *lowerRight;
//
//    QuadTreeBranchNode(
//            float xmin, float ymin, float xmax, float ymax,
//            QuadTreeNode *upperLeft,
//            QuadTreeNode *upperRight,
//            QuadTreeNode *lowerLeft,
//            QuadTreeNode *lowerRight
//    ) :
//            QuadTreeNode{xmin, ymin, xmax, ymax},
//            upperLeft{upperLeft},
//            upperRight{upperRight},
//            lowerLeft{lowerLeft},
//            lowerRight{lowerRight} {}
//
//    ~QuadTreeBranchNode() {
//        delete upperLeft;
//        delete upperRight;
//        delete lowerLeft;
//        delete lowerRight;
//    }
//
//    void writeInternals(std::ostream &ostream, int depth) {
//        ostream << "Branch node:\n";
//        upperLeft->write(ostream, depth + 1);
//        upperRight->write(ostream, depth + 1);
//        lowerLeft->write(ostream, depth + 1);
//        lowerRight->write(ostream, depth + 1);
//    };
//
//    bool isEmpty() {
//        return upperLeft->isEmpty() && upperRight->isEmpty() && lowerLeft->isEmpty() && lowerRight->isEmpty();
//    }
//
//    QuadTreeNode *add(void *ptr, float x, float y) {
//        if (!contains(x, y)) {
//            return this;
//        }
//        upperLeft = upperLeft->add(ptr, x, y);
//        upperRight = upperRight->add(ptr, x, y);
//        lowerLeft = lowerLeft->add(ptr, x, y);
//        lowerRight = lowerRight->add(ptr, x, y);
//
//        return this;
//    }
//    QuadTreeNode *remove(void *ptr, float x, float y) {
//        if (!contains(x, y)) {
//            return this;
//        }
//
//        upperLeft = upperLeft->remove(ptr, x, y);
//        upperRight = upperRight->remove(ptr, x, y);
//        lowerLeft = lowerLeft->remove(ptr, x, y);
//        lowerRight = lowerRight->remove(ptr, x, y);
//
//        return this;
//    }
//};
//
//static QuadTreeNode *assignNode(
//        bool isFirst, bool isSecond,
//        float xmin, float ymin, float xmax, float ymax,
//        QuadTreeLeafNode *current,
//        void *ptr, float x, float y
//) {
//    if (isFirst) {
//        if (isSecond) {
//            return createBranchNode(xmin, ymin, xmax, ymax, current, ptr, x, y);
//        } else {
//            current->setBounds(xmin, ymin, xmax, ymax);
//            return current;
//        }
//    } else if (isSecond) {
//        return new QuadTreeLeafNode{ptr, xmin, ymin, xmax, ymax, x, y};
//    }
//    return new QuadTreeLeafNode{nullptr, xmin, ymin, xmax, ymax, -1, -1};
//}
//
//#define BOUNDS_UL xmin, ymid, xmid, ymax
//#define BOUNDS_UR xmid, ymid, xmax, ymax
//#define BOUNDS_LL xmin, ymin, xmid, ymid
//#define BOUNDS_LR xmid, ymin, xmax, ymid
//
//static QuadTreeNode *createBranchNode(
//        float xmin, float ymin, float xmax, float ymax,
//        QuadTreeLeafNode *current,
//        T t, float x, float y
//) {
//    float xmid = (xmin + xmax) / 2;
//    float ymid = (ymin + ymax) / 2;
//
//    bool ful = is_within(BOUNDS_UL, current->x, current->y);
//    bool fur = is_within(BOUNDS_UR, current->x, current->y);
//    bool fll = is_within(BOUNDS_LL, current->x, current->y);
//    bool flr = is_within(BOUNDS_LR, current->x, current->y);
//
//    bool sul = is_within(BOUNDS_UL, x, y);
//    bool sur = is_within(BOUNDS_UR, x, y);
//    bool sll = is_within(BOUNDS_LL, x, y);
//    bool slr = is_within(BOUNDS_LR, x, y);
//
//    return new QuadTreeBranchNode{
//            xmin, ymin, xmax, ymax,
//            assignNode(ful, sul, BOUNDS_UL, current, ptr, x, y),
//            assignNode(fur, sur, BOUNDS_UR, current, ptr, x, y),
//            assignNode(fll, sll, BOUNDS_LL, current, ptr, x, y),
//            assignNode(flr, slr, BOUNDS_LR, current, ptr, x, y)
//    };
//}
//
//void QuadTree::add(T t, float x, float y) {
//    root = root->add(ptr, x, y);
//}
//
//void QuadTree::remove(T t, float x, float y) {
//    root = root->remove(ptr, x, y);
//}
//
//void QuadTree::move(T t, float xOld, float yOld, float xNew, float yNew) {
//    // TODO...
//    root = root->remove(ptr, xOld, yOld);
//    root = root->add(ptr, xNew, yNew);
//}
//
//QuadTree::~QuadTree() {
//    delete root;
//}
//
//QuadTree::QuadTree(float xmin, float ymin, float xmax, float ymax) :
//    root{new QuadTreeLeafNode{nullptr, xmin, ymin, xmax, ymax, -1, -1}} {}
//
//void QuadTree::write(std::ostream &out) {
//    root->write(out, 0);
//}
//

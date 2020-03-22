//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_QUADTREEBRANCHNODE2_H
#define PLAYING_QUADTREEBRANCHNODE2_H

#include "QuadTreePath.h"
#include "QuadTreeNode2.h"

class QuadTreeBranchNode2 : public QuadTreeNode2 {
private:
	QuadTreeNode2 *lowerLeft{nullptr};
	QuadTreeNode2 *lowerRight{nullptr};
	QuadTreeNode2 *upperLeft{nullptr};
	QuadTreeNode2 *upperRight{nullptr};

	void writeChild(JsonWriter& writer, const std::string& key, QuadTreeNode2 *child) {
		writer.writeKey(key);
		if (child == nullptr) {
			writer.writeNull();
		} else {
			child->debug(writer);
		}
	}
public:
	QuadTreeBranchNode2(
		double xmin, double ymin, double xmax, double ymax,
		qt_branch childBranch, QuadTreeNode2 *child
	);
	~QuadTreeBranchNode2();

	void set(qt_branch branch, QuadTreeNode2 *child);
	QuadTreeNode2 *get(qt_branch branch);
	QuadTreeNode2 *getOrCreate(qt_branch branch);

	void debug(JsonWriter& writer);

	void validate(int depth, int maxDepth, std::map<EntityId, Point>& locations);
};


#endif //PLAYING_QUADTREEBRANCHNODE2_H

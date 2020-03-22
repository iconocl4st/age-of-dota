//
// Created by thallock on 3/13/20.
//

#include "QuadTreeBranchNode2.h"
#include "QuadTreeLeafNode2.h"

QuadTreeBranchNode2::QuadTreeBranchNode2(
	double xmin, double ymin, double xmax, double ymax,
	qt_branch childBranch, QuadTreeNode2 *child
) : QuadTreeNode2{xmin, ymin, xmax, ymax, false},
	lowerLeft{nullptr},
	lowerRight{nullptr},
	upperLeft{nullptr},
	upperRight{nullptr}
{
	set(childBranch, child);
	size = 1;
}

QuadTreeBranchNode2::~QuadTreeBranchNode2() {
	delete lowerLeft;
	delete lowerRight;
	delete upperLeft;
	delete upperRight;
}

#define BOUNDS_UL getMinX(), getMidY(), getMidX(), getMaxY()
#define BOUNDS_UR getMidX(), getMidY(), getMaxX(), getMaxY()
#define BOUNDS_LL getMinX(), getMinY(), getMidX(), getMidY()
#define BOUNDS_LR getMidX(), getMinY(), getMaxX(), getMidY()

void QuadTreeBranchNode2::set(qt_branch branch, QuadTreeNode2 *child) {
	switch (branch) {
		case LOWER_RIGHT:
			lowerRight = child;
			if (child != nullptr) child->setBounds(BOUNDS_LR);
			break;
		case LOWER_LEFT:
			lowerLeft = child;
			if (child != nullptr) child->setBounds(BOUNDS_LL);
			break;
		case UPPER_RIGHT:
			upperRight = child;
			if (child != nullptr) child->setBounds(BOUNDS_UR);
			break;
		case UPPER_LEFT:
			upperLeft = child;
			if (child != nullptr) child->setBounds(BOUNDS_UL);
			break;
		default:
			throw std::runtime_error("error 23462727");
	}
}

QuadTreeNode2 *QuadTreeBranchNode2::get(qt_branch branch) {
	switch (branch) {
		case LOWER_RIGHT:
			return lowerRight;
		case LOWER_LEFT:
			return lowerLeft;
		case UPPER_RIGHT:
			return upperRight;
		case UPPER_LEFT:
			return upperLeft;
		default:
			throw std::runtime_error("error 1067284");
	}
}

QuadTreeNode2 *QuadTreeBranchNode2::getOrCreate(qt_branch branch) {
	QuadTreeNode2 *ret = get(branch);
	if (ret == nullptr) {
		set(branch, new QuadTreeLeafNode2(0, 0, 0, 0));
	}
	return get(branch);
}

void QuadTreeBranchNode2::debug(JsonWriter &writer) {
	writer.writeBeginObject();
	writer.writeString("type", "branch");

	writeSize(writer);
	writeBounds(writer);

	writeChild(writer, "lower-left", lowerLeft);
	writeChild(writer, "lower-right", lowerRight);
	writeChild(writer, "upper-left", upperLeft);
	writeChild(writer, "upper-right", upperRight);

	writer.writeEndObject();
}

void validate_bounds(QuadTreeNode2 *n, double xmin, double ymin, double xmax, double ymax) {
	if (n == nullptr) {
		return;
	}
	if (n->getMinX() != xmin) {
		throw std::runtime_error("");
	}
	if (n->getMinY() != ymin) {
		throw std::runtime_error("");
	}
	if (n->getMaxX() != xmax) {
		throw std::runtime_error("");
	}
	if (n->getMaxY() != ymax) {
		throw std::runtime_error("");
	}
}

void QuadTreeBranchNode2::validate(int depth, int maxDepth, std::map<EntityId, Point>& locations) {
	if (depth >= maxDepth) {
		throw std::runtime_error("validation error");
	}

	int sizeSum = 0;
	if (lowerLeft != nullptr) sizeSum += lowerLeft->getSize();
	if (lowerRight != nullptr) sizeSum += lowerRight->getSize();
	if (upperLeft != nullptr) sizeSum += upperLeft->getSize();
	if (upperRight != nullptr) sizeSum += upperRight->getSize();
	if (sizeSum != size) {
		throw std::runtime_error("validation error");
	}

	if (size <= 1) {
		throw  std::runtime_error("validation error");
	}

	validate_bounds(lowerLeft, BOUNDS_LL);
	validate_bounds(lowerRight, BOUNDS_LR);
	validate_bounds(upperLeft, BOUNDS_UL);
	validate_bounds(upperRight, BOUNDS_UR);

	if (lowerLeft != nullptr) lowerLeft->validate(depth + 1, maxDepth, locations);
	if (lowerRight != nullptr) lowerRight->validate(depth + 1, maxDepth, locations);
	if (upperLeft != nullptr) upperLeft->validate(depth + 1, maxDepth, locations);
	if (upperRight != nullptr) upperRight->validate(depth + 1, maxDepth, locations);
}

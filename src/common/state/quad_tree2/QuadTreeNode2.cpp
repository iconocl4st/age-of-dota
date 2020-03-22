//
// Created by thallock on 3/13/20.
//

#include "QuadTreeNode2.h"

QuadTreeNode2::QuadTreeNode2(double xmin, double ymin, double xmax, double ymax, bool isLeaf)  :
	min{xmin, ymin},
	max{xmax, ymax},
	size{0},
	isLeafNode{isLeaf} {}

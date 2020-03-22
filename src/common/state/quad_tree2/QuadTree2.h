//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_QUADTREE2_H
#define PLAYING_QUADTREE2_H

#include <mutex>
#include <map>
#include <boost/optional.hpp>

#include "QuadTreeNode2.h"
#include "../definitions.h"
#include "../../json/JsonDebuggable.h"
#include "QuadTreePath.h"
#include "QuadTreeBranchNode2.h"
#include "QuadTreeLeafNode2.h"

namespace aod::common::state { class CommonState; }

class QuadTree2 : public JsonDebuggable  {
private:
	QuadTreeNode2 *root;
	std::recursive_mutex mutex;
	std::map<EntityId, Point> locations;
	int maxDepth;

	QuadTreeBranchNode2 *subdivide(QuadTreeBranchNode2 *parent, qt_branch branch, QuadTreeLeafNode2 *current);
public:
	QuadTree2(double xmin, double ymin, double xmax, double ymax, int maxDepth);
	~QuadTree2();

	void add(EntityId id, const Point& location);
	void remove(EntityId id);
	void move(EntityId id, const Point& newLocation);

	boost::optional<Point> get(EntityId id) {
		auto it = locations.find(id);
		if (it == locations.end()) {
			return boost::optional<Point>{};
		}
		return boost::optional<Point>{it->second};
	}

	int size();

	Point getLocation(EntityId entityId);

	void debug(JsonWriter& writer);

	void visit(const Point& min, const Point& max, std::function<void(QuadTreeNode2*)> visitor);

	void visitEntities(aod::common::state::CommonState&, const Point &min, const Point &max, std::function<void(EntityId, const Point &p)> visitor);

	void visitLeaves(const Point& min, const Point& max, std::function<void(const Point& min, const Point& max)> visitor);

//	void visit()

	void validate();
};

#endif //PLAYING_QUADTREE2_H

//
// Created by thallock on 3/13/20.
//

#ifndef PLAYING_QUADTREENODE2_H
#define PLAYING_QUADTREENODE2_H

#include <functional>
#include <map>

#include "../Point.h"
#include "../../json/JsonDebuggable.h"
#include "../definitions.h"


class QuadTreeNode2 : public JsonDebuggable {
protected:
	Point min;
	Point max;

	int size;

	void writeSize(JsonWriter& writer) {
		writer.writeInt("size",  size);
	}
	void writeBounds(JsonWriter& writer) {
		std::ostringstream ss;
		ss << "x: [" << min.x << "-" << max.x << "], y: [" << min.y << "-" << max.y << "]";
		writer.writeString("bounds", ss.str());
	}
public:
	const bool isLeafNode;

	QuadTreeNode2(double xmin, double ymin, double xmax, double ymax, bool isLeaf);
	virtual ~QuadTreeNode2() = default;

	inline void setBounds(double xmin, double ymin, double xmax, double ymax) {
		min.x = xmin;
		min.y = ymin;
		max.x = xmax;
		max.y = ymax;
	}

	inline const Point& getMin() { return min; }
	inline const Point& getMax() { return max; }

	inline bool contains(const Point& p) {
		return !(min.x > p.x || max.x <= p.x || min.y > p.y || max.y <= p.y);
	}

	inline bool intersects(const Point& min, const Point& max) {
		return !(min.x > max.x || max.x <= min.x || min.y > max.y || max.y <= min.y);
	}

	inline void incrementSize() { ++size; }
	inline void decrementSize() { --size; }
	inline int getSize() { return size; }
	inline void clearSize() { size = 0; }

	inline double getMidX() const { return (min.x + max.x) / 2; }
	inline double getMidY() const { return (min.y + max.y) / 2; }
	inline double getMinX() const { return min.x; }
	inline double getMinY() const { return min.y; }
	inline double getMaxX() const { return max.x; }
	inline double getMaxY() const { return max.y; }

	virtual void validate(int depth, int maxDepth, std::map<EntityId, Point>& locations) = 0;

//	virtual void visit(const Point& min, const Point& max, std::function<void(QuadTreeNode2 &)> visitor) = 0;
};


#endif //PLAYING_QUADTREENODE2_H

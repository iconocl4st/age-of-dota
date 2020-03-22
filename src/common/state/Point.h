//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_POINT_H
#define PLAYING_POINT_H


#include "../json/JsonSendable.h"

class Point : public JsonSendable {
public:
    double x;
    double y;

    Point(double x, double y) : x{x}, y{y} {}
    Point() : Point(0, 0) {}

    void send(JsonWriter *writer) const {
        writer->writeBeginObject();
        writer->writeDouble("x", x);
        writer->writeDouble("y", y);
        writer->writeEndObject();
    }

    void parse(JsonReader *reader) {
        reader->readBeginObject();
        x = reader->readDouble("x");
        y = reader->readDouble("y");
        reader->readEndObject();
    }

	Point operator-(const Point &o) const {
		return Point{x - o.x, y - o.y};
    }

	void operator/=(double o) {
    	x /= o;
    	y /= o;
	}

	bool operator!=(const Point& o) const {
//		x != o.x || y != o.y;
		return !(operator==(o));
    }

	bool operator==(const Point& o) const {
		return x == o.x && y == o.y;
	}

	Point project(const Point& min, const Point& max) const {
    	return Point{
			std::max(min.x, std::min(max.x, x)),
			std::max(min.y, std::min(max.y, y))
    	};
    }

	friend std::ostream& operator<<(std::ostream& o, const Point& point) {
		o << "[x=" << point.x << ",y=" << point.y << "]";
    }

	double norm();

    void placeWithinBounds(double width, double height);
};

#endif //PLAYING_POINT_H

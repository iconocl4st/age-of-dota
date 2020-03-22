//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_SHAPE_H
#define PLAYING_SHAPE_H


#include "Point.h"

class Shape : JsonSendable {
public:
	virtual bool contains(const Point& location, double orientation, const Point& point) const = 0;
	virtual void boundingBox(const Point& location, double orientation, Point& minimum, Point& maximum) const = 0;


	virtual bool intersectsRectangle(const Point& location,  double orientation,  const Point& min, const Point& max) = 0;
	virtual bool intersectsLine(const Point& location, double orientation, const Point& lineBegin, const Point& lineEnd) const = 0;
	virtual bool intersectsGrid(const Point& location, double orientation, int gridX, int gridY) const = 0;
};


class Circle : public Shape {
private:
	double radius;

public:
	Circle(double radius) : radius{radius} {}

	bool contains(const Point& location, double orientation, const Point& point) const;
	bool intersectsLine(const Point& location, double orientation, const Point& lineBegin, const Point& lineEnd) const;
	bool intersectsRectangle(const Point& location,  double orientation,  const Point& min, const Point& max);
	bool intersectsGrid(const Point& location, double orientation, int gridX, int gridY) const;
	void boundingBox(const Point& location, double orientation, Point& minimum, Point& maximum) const;

	void send(JsonWriter *writer) const;
	void parse(JsonReader *reader);
};


class Rectangle : public Shape {
private:
	double width;
	double height;

	bool isRotated(double orientation);

public:
	Rectangle(double width, double height) : width{width}, height{height} {}

	bool contains(const Point& location, double orientation, const Point& point) const;
	bool intersectsLine(const Point& location, double orientation, const Point& lineBegin, const Point& lineEnd) const;
	bool intersectsGrid(const Point& location, double orientation, int gridX, int gridY) const;
	void boundingBox(const Point& location, double orientation, Point& minimum, Point& maximum) const;

	void send(JsonWriter *writer) const;
	void parse(JsonReader *reader);
};


#endif //PLAYING_SHAPE_H

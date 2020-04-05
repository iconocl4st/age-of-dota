//
// Created by thallock on 3/9/20.
//

#include "Shape.h"

bool Circle::intersectsLine(const Point &location, double orientation, const Point &lineBegin, const Point &lineEnd) const {
	Point p12 = lineBegin - lineEnd;
	double n = p12.norm();
	if (n <= 1e-4) {
		return contains(location, orientation, lineBegin);
	}
	p12 /= p12.norm();
	Point p1c = location - lineEnd;
	double v = std::abs(p12.x * p1c.y - p12.y * p1c.x);
	return v <= radius;

//	P1 = [-2, -2];  % Point 1 of the line
//	P2 = [2, 2];    % Point 2 of the line
//	C  = [0, 0];    % Center of circle
//	R  = 2;         % Radius of circle
//	P12 = P2 - P1;
//	N   = P12 / norm(P12);  % Normalized vector in direction of the line
//	P1C = C - P1;           % Line from one point to center
//	v   = abs(N(1) * P1C(2) - N(2) * P1C(1));  % Norm of the 2D cross-product
//	doIntersect = (v <= R);
	return false;
}

bool Circle::intersectsGrid(const Point &location, double orientation, int gridX, int gridY) const {
	return false;
}

void Circle::boundingBox(const Point &location, double orientation, Point &minimum, Point &maximum) const {

}

void Circle::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeDouble("radius", radius);
	writer->writeEndObject();
}

bool Circle::contains(const Point &location, double orientation, const Point &point) const {
	return (location - point).norm() <= radius;
}

void Circle::parse(JsonReader *reader) {
	reader->readBeginObject();
	radius = reader->readDouble("radius");
	reader->readEndObject();
}

bool Circle::intersectsRectangle(const Point &location, double orientation, const Point &min, const Point &max) {
	return (location - location.project(min, max)).norm() <= radius;
}

double Circle::distanceTo(const Point& location, double orientation, const Point& otherLocation) const {
	return std::max(0.0, (location - otherLocation).norm() - radius);
}

bool Rectangle::isRotated(double orientation) {

	return false;
}

bool Rectangle::intersectsLine(const Point &location, double orientation, const Point &lineBegin,
							   const Point &lineEnd) const {
	return false;
}

bool Rectangle::intersectsGrid(const Point &location, double orientation, int gridX, int gridY) const {
	return false;
}

bool Rectangle::contains(const Point &location, double orientation, const Point &point) const {
	return false;
}

void Rectangle::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeDouble("width", width);
	writer->writeDouble("height", height);
	writer->writeEndObject();
}

void Rectangle::parse(JsonReader *reader) {
	reader->readBeginObject();
	width = reader->readDouble("width");
	height = reader->readDouble("height");
	reader->readEndObject();
}

void Rectangle::boundingBox(const Point &location, double orientation, Point &minimum, Point &maximum) const {

}


#pragma once

#include "vector.h"
#include "Line.h"

class Circle {
public:
	vector position;
	double radius;

	Circle(const vector &position, double radius);
	Circle(double radius);

	// norm returns a vector that if added to position would make it not intersect
	bool intersects(const Circle &other, vector *norm = nullptr);
	bool intersects(const Line &other, vector *norm = nullptr);
};

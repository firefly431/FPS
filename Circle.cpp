#include "Circle.h"

#include <iostream>

Circle::Circle(const vector &position, double radius) : position(position), radius(radius) {}

Circle::Circle(double radius) : position(), radius(radius) {}

bool Circle::intersects(const Circle &other, vector *norm) {
	vector offset = position - other.position;
	double cdist = ((double)offset - radius - other.radius);
	if (norm)
		*norm = offset.normalized() * cdist;
	return cdist < 0;
}

bool Circle::intersects(const Line &other, vector *norm) {
	auto nlv = other.vec();
	double llen = (double)nlv;
	nlv /= llen;
	double t = nlv.dot(position - other.p1);
	if (t < 0) t = 0;
	if (t > llen) t = llen;
	auto proj = nlv * t + other.p1;
	auto offset = proj - position;
	double cdist = ((double)offset - radius);
	if (norm)
		*norm = offset.normalized() * (cdist - 1e-5);
	return cdist < 0;
}

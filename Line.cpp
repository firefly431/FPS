#include "Line.h"

bool Line::intersects(const Line &other, vector *intpt) const {
	vector s1(vec()), s2(other.vec());
	double det = -s2.x * s1.y + s1.x * s2.y;
	if (det == 0) return false; // parallel; asssume false
	double s = (-s1.y * (p1.x - other.p1.x) + s1.x * (p1.y - other.p1.y)) / det,
	       t = ( s2.x * (p1.y - other.p1.y) - s2.y * (p1.x - other.p1.x)) / det;
	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		*intpt = p1 + s1 * t;
		return true;
	}
	return false;
}
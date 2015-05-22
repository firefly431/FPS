#include "Circle.h"

#include <iostream>

Circle::Circle(const vector &position, double radius) : position(position), radius(radius) {}

Circle::Circle(double radius) : position(), radius(radius) {}

bool Circle::intersects(const Circle &other, vector *norm) {
    // simple circle intersection test
    vector offset = position - other.position;
    // if the distance is less than the sum of the radii
    // but we store the difference in cdist (distance of centers)
    double cdist = ((double)offset - radius - other.radius);
    if (norm)
        *norm = offset.normalized() * cdist;
    return cdist < 0;
}

bool Circle::intersects(const Line &other, vector *norm) {
    // normalized line vector (not normalized yet)
    auto nlv = other.vec();
    // length of the line
    double llen = (double)nlv;
    nlv /= llen; // normalize it
    // projection of the position in line-space onto the line
    double t = nlv.dot(position - other.p1);
    // cap at [0, llen]
    if (t < 0) t = 0;
    if (t > llen) t = llen;
    // project the point back
    auto proj = nlv * t + other.p1;
    // distance between the projection and the position
    auto offset = proj - position;
    double cdist = ((double)offset - radius);
    if (norm)
        // the 1e-5 is a fudge factor to prevent it from recomputing false collisions
        *norm = offset.normalized() * (cdist - 1e-5);
    return cdist < 0;
}

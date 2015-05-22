#pragma once

#include "Shape.h"

// triangle class for intersection
class Triangle : public Shape {
    friend class Scene;
    friend class Quad;
public:
    Triangle(const vector &, const vector &, const vector &);
    bool contains(const vector &) const override;
protected:
    // points
    const vector a, b, c;
    // line vectors (used in intersection test)
    const vector v0, v1;
    // dots of line vectors (used in intersection test)
    const double d00, d01, d11, denom;
    // bounds (used in intersection test)
    const double minx, maxx, miny, maxy;
    bool hasPoint(const vector &v) const override;
};

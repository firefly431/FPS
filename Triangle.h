#pragma once

#include "Shape.h"

class Triangle : public Shape {
    friend class Scene;
    friend class Quad;
public:
    Triangle(const vector &, const vector &, const vector &);
    bool contains(const vector &) const override;
protected:
    const vector a, b, c;
    const vector v0, v1;
    const double d00, d01, d11, denom;
    const double minx, maxx, miny, maxy;
    bool hasPoint(const vector &v) const override;
};

#pragma once

#include "vector.h"

// Line class for collision
class Line
{
public:
    vector p1, p2;

    inline Line(const vector &p1, const vector &p2) : p1(p1), p2(p2) {}

    // line vector
    inline vector vec() const {
        return p2 - p1;
    }

    // intpt is the intersection point
    // set to null to not set
    bool intersects(const Line &other, vector *intpt) const;
};


#pragma once

#include "vector.h"

class Line
{
public:
    vector p1, p2;

    inline Line(const vector &p1, const vector &p2) : p1(p1), p2(p2) {}

    inline vector vec() const {
        return p2 - p1;
    }

    bool intersects(const Line &other, vector *intpt) const;
};


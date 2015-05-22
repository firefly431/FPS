#pragma once

#include "vector.h"

// collision shape class
class Shape {
    friend class Scene;
    // does the shape have this point as one of its vertices
    virtual bool hasPoint(const vector &v) const = 0;
public:
    // does the shape contain the point
    virtual bool contains(const vector &v) const = 0;
};

#pragma once

#include "vector.h"

class Shape {
    friend class Scene;
    virtual bool hasPoint(const vector &v) const = 0;
public:
    virtual bool contains(const vector &v) const = 0;
};

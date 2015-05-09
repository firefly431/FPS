#pragma once

#include "vector.h"

class Shape {
public:
    virtual bool contains(const vector &v) const = 0;
};

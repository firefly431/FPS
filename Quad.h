#pragma once

#include "Triangle.h"

class Quad : public Shape {
public:
    Quad(const vector &, const vector &, const vector &, const vector &);
    bool contains(const vector &) const override;
protected:
    const Triangle a, b;
};


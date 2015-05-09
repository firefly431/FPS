#pragma once

#include "Triangle.h"

class Quad : public Shape {
    friend class Scene;
public:
    Quad(const vector &, const vector &, const vector &, const vector &);
    bool contains(const vector &) const override;
protected:
    const Triangle a, b;
    bool hasPoint(const vector &) const override;
};


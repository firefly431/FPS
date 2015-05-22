#pragma once

#include "Triangle.h"

// quadrilateral intersection shape
class Quad : public Shape {
    friend class Scene;
public:
    // four points
    Quad(const vector &, const vector &, const vector &, const vector &);
    bool contains(const vector &) const override;
protected:
    // just consists of two triangles a and b which are checked
    const Triangle a, b;
    bool hasPoint(const vector &) const override;
};


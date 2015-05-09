#include "Quad.h"

Quad::Quad(const vector &a, const vector &b, const vector &c, const vector &d) : a(a, b, c), b(c, d, a) {}

bool Quad::contains(const vector &p) const {
    return a.contains(p) || b.contains(p);
}

bool Quad::hasPoint(const vector &p) const {
    return a.hasPoint(p) || b.hasPoint(p);
}


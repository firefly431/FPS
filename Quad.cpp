#include "Quad.h"

Quad::Quad(const vector &a, const vector &b, const vector &c, const vector &d) : a(a, b, c), b(b, c, d) {}

bool Quad::contains(const vector &p) const {
    return a.contains(p) || b.contains(p);
}


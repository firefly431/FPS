#include "Triangle.h"

Triangle::Triangle(const vector &a, const vector &b, const vector &c) : a(a), b(b), c(c), v0(b - a), v1(c - a), d00(v0.sqr()), d01(v0 * v1), d11(v1.sqr()), denom(d00 * d11 - d01 * d01) {}

bool Triangle::contains(const vector &p) const {
    vector v2 = p - a;
    const double d20 = v2 * v0;
    const double d21 = v2 * v1;
    const double v = (d11 * d20 - d01 * d21) / denom,
                 w = (d00 * d21 - d01 * d20) / denom,
                 u = 1 - v - w;
    return v >= 0 && v <= 1 && w >= 0 && w <= 1 && u >= 0 && u <= 1;
}

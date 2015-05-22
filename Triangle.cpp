#include "Triangle.h"

static const double EPSILON = 0.1;
static const double EPSQ = EPSILON * EPSILON;

static inline double min_(double a, double b, double c) {
    return (a < b ? (a < c ? a : c) : (b < c ? b : c)) - EPSILON;
}

static inline double max_(double a, double b, double c) {
    return (a > b ? (a > c ? a : c) : (b > c ? b : c)) + EPSILON;
}

// distance squared point to segment
inline double dspts(const vector &a, const vector &b, const vector &p) {
    double sql = (b - a).sqr();
    double dot = ((p - a) * (b - a)) / sql;
    if (dot < 0)
        return (p - a).sqr();
    if (dot <= 1) {
        double psq = (p - a).sqr();
        return psq - dot * dot * sql;
    }
    return (p - b).sqr();
}

// is included
inline bool dsptsi(const vector &a, const vector &b, const vector &p) {
    return dspts(a, b, p) <= EPSQ;
}

Triangle::Triangle(const vector &a,
                   const vector &b,
                   const vector &c)
    : a(a), b(b), c(c),
      v0(b - a), v1(c - a), // calculate a bunch of things
      d00(v0.sqr()), d01(v0 * v1), d11(v1.sqr()),
      denom(d00 * d11 - d01 * d01),
      minx(min_(a.x, b.x, c.x)), maxx(max_(a.x, b.x, c.x)),
      miny(min_(a.y, b.y, c.y)), maxy(max_(a.y, b.y, c.y))
{}

bool Triangle::contains(const vector &p) const {
    // crude AABB test
    if (p.x < minx || p.x > maxx || p.y < miny || p.y > maxy)
        return false;
    // barycentric coordinate test
    vector v2 = p - a;
    const double d20 = v2 * v0;
    const double d21 = v2 * v1;
    const double v = (d11 * d20 - d01 * d21) / denom,
                 w = (d00 * d21 - d01 * d20) / denom,
                 u = 1 - v - w;
    if (v >= 0 && v <= 1 && w >= 0 && w <= 1 && u >= 0 && u <= 1)
        return true;
    // check outlines
    return dsptsi(a, b, p) || dsptsi(b, c, p) || dsptsi(c, a, p);
}

bool Triangle::hasPoint(const vector &p) const {
    return a == p || b == p || c == p;
}

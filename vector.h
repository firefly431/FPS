#ifndef _INCLUDED_VECTOR_H
#define _INCLUDED_VECTOR_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993752
#endif

struct vector {
    double x, y;
    inline vector() : x(0), y(0) {}
    inline vector(const double x, const double y) : x(x), y(y) {}
    inline vector(const double angle) : x(std::cos(angle)), y(std::sin(angle)) {}
    inline vector(const vector &other): x(other.x), y(other.y) {}

    inline vector &operator=(const vector &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    inline vector operator+(const vector &other) const {
        return vector(other.x + x, other.y + y);
    }

    inline vector operator-(const vector &other) const {
        return vector(x - other.x, y - other.y);
    }

    // actually dot product
    inline double operator*(const vector &other) const {
        return other.x * x + other.y * y;
    }

    inline vector operator/(const vector &other) const {
        return vector(x / other.x, y / other.y);
    }

    inline vector operator*(const double fac) const {
        return vector(x * fac, y * fac);
    }

    inline vector operator/(const double fac) const {
        return vector(x / fac, y / fac);
    }

    inline vector &operator+=(const vector &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline vector &operator-=(const vector &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline vector &operator*=(const vector &other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    inline vector &operator/=(const vector &other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    inline vector &operator*=(const double other) {
        x *= other;
        y *= other;
        return *this;
    }

    inline vector &operator/=(const double other) {
        x /= other;
        y /= other;
        return *this;
    }

    inline operator double(void) const {
        return std::sqrt(x * x + y * y);
    }

    inline double sqr(void) const {
        return x * x + y * y;
    }

    inline void normalize(void) {
        (*this) /= (double)(*this);
    }

    inline vector normalized(void) const {
        return (*this) / (double)(*this);
    }

    inline double angle(void) const {
        return std::atan2(y, x);
    }

    inline double dist(const vector &other) const {
        return (*this - other);
    }

    inline double dot(const vector &other) const {
        return x * other.x + y * other.y;
    }

    // TODO: project point onto line
};

#endif

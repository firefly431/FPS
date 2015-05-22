// I used actual include guards here instead of a #pragma once
#ifndef _INCLUDED_VECTOR_H
#define _INCLUDED_VECTOR_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993752
#endif

// vector class
// has x and y components
struct vector {
    double x, y;
    inline vector() : x(0), y(0) {}
    inline vector(const double x, const double y) : x(x), y(y) {}
    inline explicit vector(const double angle) : x(std::cos(angle)), y(std::sin(angle)) {}
    inline vector(const vector &other): x(other.x), y(other.y) {} // copy ctor

    inline vector &operator=(const vector &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    // equality
    inline bool operator==(const vector &other) const {
        return x == other.x && y == other.y;
    }

    // vector addition/subtraction
    inline vector operator+(const vector &other) const {
        return vector(other.x + x, other.y + y);
    }

    inline vector operator-(const vector &other) const {
        return vector(x - other.x, y - other.y);
    }

    // actually dot product
    // dot product is defined as
    // |A| |B| cos \theta
    // where \theta is the angle between them
    inline double operator*(const vector &other) const {
        return other.x * x + other.y * y;
    }

    // component divide
    inline vector operator/(const vector &other) const {
        return vector(x / other.x, y / other.y);
    }

    // multiplication/division by a scalar
    inline vector operator*(const double fac) const {
        return vector(x * fac, y * fac);
    }

    inline vector operator/(const double fac) const {
        return vector(x / fac, y / fac);
    }

    // assignment stuff
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

    // magnitude
    inline explicit operator double(void) const {
        return std::sqrt(x * x + y * y);
    }

    // square of the magnitude
    // note: I often use a.sqr() < DIST * DIST
    // as a small optimization to avoid
    // a costly sqrt
    inline double sqr(void) const {
        return x * x + y * y;
    }

    // normalize a vector
    inline void normalize(void) {
        (*this) /= (double)(*this);
    }

    // get the vector normalized (does not modify)
    inline vector normalized(void) const {
        return (*this) / (double)(*this);
    }

    // get the vector's angle with horizontal
    inline double angle(void) const {
        return std::atan2(y, x);
    }

    // distance between two vectors
    inline double dist(const vector &other) const {
        return (double)(*this - other);
    }

    // explicit dot product
    inline double dot(const vector &other) const {
        return x * other.x + y * other.y;
    }
};

#endif

#ifndef _INCLUDED_MATRIX_H
#define _INCLUDED_MATRIX_H

#include <cmath>
#include "vec4.h"

// 4x4 matrix
// note that in OpenGL, matrices are COLUMN-MAJOR
struct matrix {
    double data[4][4];
    // identity
    matrix() : data(0) {
        data[0][0] = data[1][1] = data[2][2] = data[3][3] = 1;
    }

    inline double *operator[](size_t idx) {
        return data[idx];
    }

    inline static matrix translate(double x, double y, double z) {
        matrix ret;
        ret[3][0] = x;
        ret[3][1] = y;
        ret[3][2] = z;
        return ret;
    }

    inline static matrix scale(double x, double y, double z) {
        matrix ret;
        ret[0][0] = x;
        ret[1][1] = y;
        ret[2][2] = z;
        return ret;
    }

    inline static matrix rotateX(double x) {
        matrix ret;
        ret[1][1] = std::cos(x);
        ret[2][1] = -std::sin(x);
        ret[1][2] = std::sin(x);
        ret[2][2] = std::cos(x);
        return ret;
    }

    inline static matrix rotateY(double x) {
        matrix ret;
        ret[0][0] = std::cos(x);
        ret[2][0] = std::sin(x);
        ret[0][2] = -std::sin(x);
        ret[2][2] = std::cos(x);
        return ret;
    }

    inline static matrix rotateZ(double x) {
        matrix ret;
        ret[0][0] = std::cos(x);
        ret[1][0] = -std::sin(x);
        ret[0][1] = std::sin(x);
        ret[1][1] = std::cos(x);
        return ret;
    }

    inline vec4 operator*(const vec4 &v) {
        return vec4(data[0][0] * v.x + data[1][0] * v.y + data[2][0] * v.z + data[3][0] * v.w,
            data[0][1] * v.x + data[1][1] * v.y + data[2][1] * v.z + data[3][1] * v.w,
            data[0][2] * v.x + data[1][2] * v.y + data[2][2] * v.z + data[3][2] * v.w,
            data[0][3] * v.x + data[1][3] * v.y + data[2][3] * v.z + data[3][3] * v.w);
    }
};

#endif
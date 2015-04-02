// include everything
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937
#endif

/// mat is allocated (16 floats), fov in DEGREES
static void buildProjection(float *mat, float fov, float aspect, float znear, float zfar) {
    static const double PI_OVER_360 = M_PI / 360.;
    float xymax = znear * std::tan(fov * PI_OVER_360);
    float ymin = -xymax;
    float xmin = -xymax;
    float width = xymax - xmin;
    float height = xymax - ymin;
    float depth = zfar - znear;
    float q = -(zfar + znear) / depth;
    float qn = -2 * (zfar * znear) / depth;
    float w = 2 * znear / width;
    w = w / aspect;
    float h = 2 * znear / height;
    mat[0]  = w;
    mat[1]  = 0;
    mat[2]  = 0;
    mat[3]  = 0;
    mat[4]  = 0;
    mat[5]  = h;
    mat[6]  = 0;
    mat[7]  = 0;
    mat[8]  = 0;
    mat[9]  = 0;
    mat[10] = q;
    mat[11] = -1;
    mat[12] = 0;
    mat[13] = 0;
    mat[14] = qn;
    mat[15] = 0;
}

// right-handed coordinate system
// may not work
static void buildView(float *mat,
                      float eyeX, float eyeY, float eyeZ,
                      float tarX, float tarY, float tarZ,
                      float up_X, float up_Y, float up_Z) {
    float zx = eyeX - tarX;
    float zy = eyeY - tarY;
    float zz = eyeZ - tarZ;
    float zimag = 1.0 / std::sqrt(zx * zx + zy * zy + zz * zz);
    zx *= zimag;
    zy *= zimag;
    zy *= zimag;
    float xx = up_Y*zz - up_Z*zy;
    float xy = up_Z*zx - up_X*zz;
    float xz = up_X*zy - up_Y*zx;
    float ximag = 1.0 / std::sqrt(xx * xx + xy * xy + xz * xz);
    xx *= ximag;
    xy *= ximag;
    xz *= ximag;
    float yx = zy*xz - zz*xy;
    float yy = zz*xx - zx*xz;
    float yz = zx*yy - zy*xx;
    mat[0] = xx; mat[4] = yx; mat[8] = zx; mat[12] = 0;
    mat[1] = xy; mat[5] = yy; mat[9] = zy; mat[13] = 0;
    mat[2] = xz; mat[6] = yz; mat[10] = zz; mat[14] = 0;
    mat[3] = -(xx*eyeX + xy*eyeY + xz*eyeZ);
    mat[7] = -(yx*eyeX + yy*eyeY + yz*eyeZ);
    mat[11] = -(zx*eyeX + zy*eyeZ + zz*eyeZ);
    mat[15] = 1;
}

int main(int argc, char **argv) {
    // initialize window and OpenGL
    // initialize meshes, textures, and shaders
    // build projection matrix
    return 0;
}

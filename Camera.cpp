#include "Camera.h"

#include <cmath>

#include "BindingPoint.h"
#include "Player.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937
#endif

Camera::Camera() : binding_point(BindingPoint::next()),
    ubo(sizeof(GLfloat) * 32, GL_DYNAMIC_DRAW, BindingPoint::next()) {}
Camera::Camera(Camera &&move) : binding_point(move.binding_point), ubo(std::move(move.ubo)) {
    move.binding_point = 0;
}

void Camera::updateView(float eyeX, float eyeY, float eyeZ,
                        float dirX, float dirY, float dirZ,
                        float up_X, float up_Y, float up_Z) {
    GLfloat *mat = (GLfloat *)ubo.data;
    float zx = -dirX;
    float zy = -dirY;
    float zz = -dirZ;
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
    mat[0] = xx; mat[1] = yx; mat[2] = zx; mat[3] = 0;
    mat[4] = xy; mat[5] = yy; mat[6] = zy; mat[7] = 0;
    mat[8] = xz; mat[9] = yz; mat[10] = zz; mat[11] = 0;
    mat[12] = -(xx*eyeX + xy*eyeY + xz*eyeZ);
    mat[13] = -(yx*eyeX + yy*eyeY + yz*eyeZ);
    mat[14] = -(zx*eyeX + zy*eyeY + zz*eyeZ);
    mat[15] = 1;
    ubo.activate();
    ubo.update(0, sizeof(GLfloat) * 16);
}

void Camera::updateView(const Player &player, float eyeZ) {
    updateView(player.position.x, player.position.y, eyeZ,
               std::cos(player.heading), std::sin(player.heading), 0,
               0, 0, 1);
}

void Camera::updateProj(float fov, float aspect, float znear, float zfar) {
    GLfloat *mat = ((GLfloat *)ubo.data) + 16;
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
    ubo.activate();
    ubo.update(sizeof(GLfloat) * 16, sizeof(GLfloat) * 16);
}

GLint Camera::getBindingPoint() const {
    return binding_point;
}

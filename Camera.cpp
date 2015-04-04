#include "Camera.h"

#include <cmath>

#include "BindingPoint.h"
#include "Player.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937
#endif

#include <cstdlib>

Camera::Camera() : view(), proj() {}

Camera::Camera(Camera &&move) {
    std::memcpy(view, move.view, sizeof(GLfloat) * 16);
    std::memcpy(proj, move.proj, sizeof(GLfloat) * 16);
}

void Camera::updateView(float eyeX, float eyeY, float eyeZ,
                        float dirX, float dirY, float dirZ,
                        float up_X, float up_Y, float up_Z) {
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
    float yz = zx*xy - zy*xx;
    view[0] = xx; view[1] = yx; view[2] = zx; view[3] = 0;
    view[4] = xy; view[5] = yy; view[6] = zy; view[7] = 0;
    view[8] = xz; view[9] = yz; view[10] = zz; view[11] = 0;
    view[12] = -(xx*eyeX + xy*eyeY + xz*eyeZ);
    view[13] = -(yx*eyeX + yy*eyeY + yz*eyeZ);
    view[14] = -(zx*eyeX + zy*eyeY + zz*eyeZ);
    view[15] = 1;
}

void Camera::updateView(const Player &player, float eyeZ) {
    updateView(player.position.x, player.position.y, eyeZ,
               std::cos(player.heading), std::sin(player.heading), 0,
               0, 0, 1);
}

void Camera::updateProj(float fov, float aspect, float znear, float zfar) {
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
    proj[0]  = w;
    proj[1]  = 0;
    proj[2]  = 0;
    proj[3]  = 0;
    proj[4]  = 0;
    proj[5]  = h;
    proj[6]  = 0;
    proj[7]  = 0;
    proj[8]  = 0;
    proj[9]  = 0;
    proj[10] = q;
    proj[11] = -1;
    proj[12] = 0;
    proj[13] = 0;
    proj[14] = qn;
    proj[15] = 0;
}

void Camera::setViewUniform(GLint loc) const {
    glUniformMatrix4fv(loc, 1, GL_FALSE, view);
}

void Camera::setProjUniform(GLint loc) const {
    glUniformMatrix4fv(loc, 1, GL_FALSE, proj);
}

#pragma once

#include "OpenGL.h"
#include "Buffer.h"

class Player;

// holds camera matrices
class Camera {
public:
    Camera();
    Camera(Camera &&move);
    // update the view matrix with the specified axes
    void updateView(float eyeX, float eyeY, float eyeZ,
                    float dirX, float dirY, float dirZ,
                    float up_X, float up_Y, float up_Z);
    // update the view matrix with the player's direction and location
    void updateView(const Player &player, float eyeZ);
    // update the projection matrix
    void updateProj(float fov, float aspect, float znear, float zfar);
    // set the uniform indicated to the matrix
    void setViewUniform(GLint loc) const;
    void setProjUniform(GLint loc) const;
protected:
    GLfloat view[16], proj[16];
};

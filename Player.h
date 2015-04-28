#pragma once

#include "vector.h"

/*
 * Transformation matrix:
 * [  cos(x) -sin(x)       0       x
 *    sin(x)  cos(x)       0       y
 *         0       0       1       z
 *         0       0       0       1 ]
 */
class Player {
    friend class PlayerMesh;
    friend class Camera;
public:
    Player();
    Player(const vector pos, const double h);
    void rotate(double amount);
    void moveForward();
    void moveLeft();
    void moveRight();
    void moveBack();
    static double MOVEMENT_SPEED, SIDE_SPEED, BACK_SPEED;
    static double ZERO_ANGLE;
protected:
    vector position;
    double heading;
    double getRotation() const;
};

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
public:
    Player();

    // game logic stuff; will be implemented after graphics
protected:
    vector position;
    double heading;
};

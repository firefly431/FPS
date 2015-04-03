#pragma once

#include "vector.h"

class Player {
public:
    Player();

    // game logic stuff; will be implemented after graphics
protected:
    vector position;
    double heading;
};

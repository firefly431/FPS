#pragma once

#include "vector.h"

#include <vector>

class Player;
class Line;

// spear class
class Spear {
public:
    // head position
    vector head;
    // velocity vector
    vector velocity;
    // who owns it
    Player *owner;

    const static double SPEAR_SPEED;

    Spear(vector head, double direction, Player *owner);

    bool move(std::vector<Player> &players, const std::vector<Line> &walls);

    Line getCollisionBounds() const;
};

#pragma once

#include "vector.h"

#include <vector>

class Player;
class Line;

class Spear {
public:
    vector head;
    vector velocity;
    Player *owner;

    const static double SPEAR_SPEED; // = 10

    Spear(vector head, double direction, Player *owner);

    bool move(std::vector<Player> &players, const std::vector<Line> &walls);

    Line getCollisionBounds() const;
};

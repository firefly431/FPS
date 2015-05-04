#pragma once

#include "vector.h"

#include <vector>

class Player;
class Line;

class Spear {
    vector head;
    vector velocity;
public:
    const static double SPEAR_SPEED; // = 10

    Spear(vector head, double direction);

    void move(std::vector<Player> &players, const std::vector<Line> &walls);

    Line getCollisionBounds() const;
};
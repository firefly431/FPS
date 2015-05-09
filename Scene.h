#pragma once

#include <vector>
#include <list>

#include "Player.h"
#include "Spear.h"
#include "Line.h"

// the model class
class Scene {
    friend class TopDownRenderer;
    std::vector<Player> players;
    std::list<Spear> spears;
    std::vector<Line> walls;
public:
    void loadWalls(const std::string &fname);
    void update();
    vector addPlayer(); // returns spawn point
};

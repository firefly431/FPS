#pragma once

#include <vector>

#include "Player.h"
#include "Spear.h"
#include "Line.h"

// the model class
class Scene {
    friend class TopDownRenderer;
    std::vector<Player> players;
    std::vector<Spear> spears;
    std::vector<Line> walls;
    std::vector<vector> spawnPoints;
public:
    // common methods
    // server methods
    void loadStage(const std::string &fname); // load walls and spawn points
    void setPlayerInput(int idx, bool up, bool down, bool left, bool right, double rotation);
    void update();
    vector addPlayer(); // returns spawn point
};

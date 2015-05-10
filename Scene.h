#pragma once

#include <vector>
#include <list>

#include "Player.h"
#include "Spear.h"
#include "Line.h"
#include "MapGraph.h"

// the model class
class Scene {
public:
    std::vector<Player> players;
    std::list<Spear> spears;
    std::vector<Line> walls;
    MapGraph graph;
    void loadWalls(const std::string &fname);
    void loadGraph(const std::string &fname);
    void update();
    vector addPlayer(); // returns spawn point
protected:
    bool wouldCollide(const vector &);
};

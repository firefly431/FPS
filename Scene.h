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
    // pathfinding graph
    MapGraph graph;
    // such as A-B-C-D-A
    std::list<std::list<vector>> islands;
    // load a graph from a file
    void loadGraph(const std::string &fname);
    vector addPlayer(); // returns spawn point
protected:
    // would a point collide with any players
    bool wouldCollide(const vector &);
};

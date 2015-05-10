#pragma once

#include "PlayerController.h"
#include "MapGraph.h"

#include <list>
#include <vector>
#include "Line.h"

class Player;

class AIController : public PlayerController {
public:
    AIController(Player &target, MapGraph &nodes, std::vector<Line> &walls);
    static const double DISTANCE;
    static const double TARGET_DISTANCE;
    static const double BUFFER;
    static const double FOV;
    static const double GROSS_ROTATION;
    static const double GROSS_THRESHOLD;
    static const double ROTATION_SPEED;
    static const int REACTION_TIME;
protected:
    typedef MapGraph::Node *NodeRef;
    void update(Player &) override;
    Player &target;
    MapGraph &nodes;
    std::vector<Line> &walls;
    std::list<NodeRef> path;
    NodeRef find(vector &target);
    void walk(Player &);
    void shoot(Player &);
    void shootAt(Player &, const vector &);
    void face(Player &, const vector &);
    bool turnTowards(Player &, const vector &);
    void calculate_path(NodeRef, NodeRef);
    int reaction_timer;
};

#pragma once

#include "PlayerController.h"
#include "MapGraph.h"

#include <list>

class Player;

class AIController : public PlayerController {
public:
    AIController(Player &target, MapGraph &nodes);
protected:
    typedef MapGraph::Node *NodeRef;
    void update(Player &) override;
    Player &target;
    MapGraph &nodes;
    std::list<NodeRef> path;
    NodeRef find(vector &target);
    void walk(Player &);
    void shoot(Player &);
    void face(Player &, const vector &);
    void calculate_path(NodeRef, NodeRef);
};

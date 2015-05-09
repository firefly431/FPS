#pragma once

#include "PlayerController.h"
#include "MapGraph.h"

#include <list>

class Player;

class AIController : public PlayerController {
    friend class TopDownRenderer;
public:
    AIController(Player &target, MapGraph &nodes);
    static const double DISTANCE;
    static const double BUFFER;
protected:
    typedef MapGraph::Node *NodeRef;
    void update(Player &) override;
    Player &target;
    MapGraph &nodes;
    std::list<NodeRef> path;
    NodeRef find(vector &target);
    void walk(Player &);
    void shoot(Player &);
    void shootAt(Player &, const vector &);
    void face(Player &, const vector &);
    void calculate_path(NodeRef, NodeRef);
};

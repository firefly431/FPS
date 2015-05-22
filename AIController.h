#pragma once

#include "PlayerController.h"
#include "MapGraph.h"

#include <list>
#include <vector>
#include "Line.h"

class Player;

// AI controller
class AIController : public PlayerController {
public:
    AIController(Player &target, MapGraph &nodes, std::vector<Line> &walls);
    // preferred distance between me and player
    static const double DISTANCE;
    // distance until pathfinding
    static const double TARGET_DISTANCE;
    // acceptable buffer distance
    static const double BUFFER;
    // field of view (radians)
    static const double FOV;
    // gross rotation speed
    static const double GROSS_ROTATION;
    // threshold until a rotation is considered "gross"
    static const double GROSS_THRESHOLD;
    // speed of rotations (rad/frame)
    static const double ROTATION_SPEED;
    // frames of reaction
    static const int REACTION_TIME;
protected:
    // we use pointers to nodes a lot so we typedefed it
    typedef MapGraph::Node *NodeRef;
    // implement method
    void update(Player &) override;
    // target player
    Player &target;
    // pathfinding graph
    MapGraph &nodes;
    // list of walls
    std::vector<Line> &walls;
    // contains the path to follow
    std::list<NodeRef> path;
    // find the node that contains target
    NodeRef find(vector &target);
    // walk forward
    void walk(Player &);
    // shoot
    void shoot(Player &);
    // shoot at a thing
    void shootAt(Player &, const vector &);
    // immediately face
    void face(Player &, const vector &);
    // turn towards
    bool turnTowards(Player &, const vector &);
    // calculate the path between nodes
    void calculate_path(NodeRef, NodeRef);
    // timer for reaction
    int reaction_timer;
};

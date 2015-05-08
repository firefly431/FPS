#pragma once

#include "PlayerController.h"

class AIController : public PlayerController {
public:
    AIController(Player &target, Graph<MapNode> &nodes);
protected:
    void update(Player &) override;
    Player &target;
    Graph<MapNode> &nodes;
};
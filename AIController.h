#pragma once

#include "PlayerController.h"
#include "MapGraph.h"

class AIController : public PlayerController {
public:
    AIController(Player &target, MapGraph &nodes);
protected:
    void update(Player &) override;
    Player &target;
    MapGraph &nodes;
};
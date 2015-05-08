#pragma once

class Player;

class PlayerController {
protected:
    friend class Player;
    virtual void update(Player &p) = 0;
};
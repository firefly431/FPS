#pragma once

class Player;

// abstract class for player controller (AI)
class PlayerController {
protected:
    friend class Player;
    virtual void update(Player &p) = 0;
};

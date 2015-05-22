#pragma once

#include <vector>
#include <list>
#include "vector.h"
#include "Circle.h"

#include "PlayerController.h"

class Spear;

/*
 * Transformation matrix:
 * [  cos(x) -sin(x)       0       x
 *    sin(x)  cos(x)       0       y
 *         0       0       1       z
 *         0       0       0       1 ]
 */
class Player {
    friend class PlayerMesh;
    friend class Camera;
public:
    Player();
    Player(const vector pos, const double h);

    vector position;
    double heading;
    double movement_speed; // scale, not real movement speed
    int fire_rate; // timer in frames

    struct {
        bool up, left, right, down, fire;
    } input;

    void move(const std::vector<Line> &walls, std::list<Spear> &spears);

    Circle getCollisionBounds() const;

    // self-explanatory
    static const double MOVEMENT_SPEED, SIDE_SPEED, BACK_SPEED;
    // angle that is zero in the mesh (pointing forwards)
    static const double ZERO_ANGLE;
    // radius of collision
    static const double COLLISION_RADIUS;
    // eye level (Z)
    static const double EYE_LEVEL;
    // fire rate in frames
    static const int FIRE_RATE;
    // mesh rotation, not player rotation
    double getRotation() const;
    void rotate(double amount);
    // called when hit
    void hit(const Spear &spear);
    void setController(PlayerController *);
    std::unique_ptr<PlayerController> controller;
protected:
    void moveForward();
    void moveLeft();
    void moveRight();
    void moveBack();
};

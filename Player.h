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
    friend class TopDownRenderer;
public:
    Player();
    Player(const vector pos, const double h);

    vector position;
    double heading;
    double movement_speed; // scale
    int fire_rate;

    struct {
        bool up, left, right, down, fire;
    } input;

    void move(const std::vector<Line> &walls, std::list<Spear> &spears);

    Circle getCollisionBounds() const;

    static const double MOVEMENT_SPEED, SIDE_SPEED, BACK_SPEED;
    static const double ZERO_ANGLE;
    static const double COLLISION_RADIUS;
    static const double EYE_LEVEL;
    static const int FIRE_RATE;
    double getRotation() const;
    void rotate(double amount);
    void hit(const Spear &spear);
    void setController(PlayerController *);
    std::unique_ptr<PlayerController> controller;
protected:
    void moveForward();
    void moveLeft();
    void moveRight();
    void moveBack();
};

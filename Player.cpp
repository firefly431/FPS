#include "Player.h"

#include "Spear.h"
#include "PlayerController.h"

#include <iostream>

const double Player::MOVEMENT_SPEED = 0.3;
const double Player::SIDE_SPEED = 0.1;
const double Player::BACK_SPEED = 0.1;
const double Player::ZERO_ANGLE = M_PI;
const double Player::COLLISION_RADIUS = 1;
const double Player::EYE_LEVEL = 1;
const int Player::FIRE_RATE = 30;

Player::Player() : position(0, 0), heading(0), movement_speed(1.0), fire_rate(0), input(), controller() {
    input.up = input.down = input.left = input.right = input.fire = false;
}
Player::Player(const vector pos, const double h) : position(pos), heading(h), movement_speed(1.0), fire_rate(0), input(), controller() {
    input.up = input.down = input.left = input.right = input.fire = false;
}

void Player::rotate(double amount) {
    heading += amount;
    // normalize to 0..2pi
    heading = fmod(heading, 2 * M_PI);
    if (heading < 0)
        heading += 2 * M_PI;
}

void Player::moveForward() {
    position += vector(heading) * Player::MOVEMENT_SPEED * movement_speed;
}

void Player::moveLeft() {
    position += vector(heading + M_PI / 2) * Player::SIDE_SPEED * movement_speed;
}

void Player::moveRight() {
    position += vector(heading - M_PI / 2) * Player::SIDE_SPEED * movement_speed;
}

void Player::moveBack() {
    position += vector(heading) * -Player::BACK_SPEED * movement_speed;
}

double Player::getRotation() const {
    return heading - Player::ZERO_ANGLE;
}

void Player::move(const std::vector<Line> &walls, std::list<Spear> &spears) {
    // if we have a controller let it update our controls
    if (controller)
        controller->update(*this);
    if (input.up) moveForward();
    if (input.down) moveBack();
    if (input.left) moveLeft();
    if (input.right) moveRight();
    Circle circ(getCollisionBounds());
    if (input.fire) {
        if (fire_rate <= 0) {
            fire_rate = FIRE_RATE;
            spears.emplace_back(position, heading, this);
        } else {
            fire_rate--;
        }
    }
    else // clamp fire_rate at -1
        if (--fire_rate < 0) fire_rate = -1;
    int colcount = 0; // collision count
    if (false) { // only update count when redoing
redo_collision:
        if (++colcount > 20) {
            std::cerr << "Over 20 collisions; aborting" << std::endl;
            return;
        }
        circ = getCollisionBounds();
    }
    // intersect all the walls
    for (const auto &wall : walls) {
        vector off;
        if (circ.intersects(wall, &off)) {
            position += off;
            goto redo_collision;
        }
    }
}

Circle Player::getCollisionBounds() const {
    return Circle(position, COLLISION_RADIUS);
}

void Player::hit(const Spear &spear) {
    // do nothing
    // was planning on doing something
    // but alas, was never implemented
}

void Player::setController(PlayerController *ct) {
    controller.reset(ct);
}

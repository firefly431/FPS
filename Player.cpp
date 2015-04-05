#include "Player.h"

double Player::MOVEMENT_SPEED = 0.3;
double Player::SIDE_SPEED = 0.1;
double Player::BACK_SPEED = 0.1;
double Player::ZERO_ANGLE = M_PI;

Player::Player() : position(0, 0), heading(0) {}
Player::Player(const vector pos, const double h) : position(pos), heading(h) {}

void Player::rotate(double amount) {
    heading += amount;
    // normalize to 0..2pi
    heading = fmod(heading, 2 * M_PI);
    if (heading < 0)
        heading += 2 * M_PI;
}

void Player::moveForward() {
    position += vector(heading) * Player::MOVEMENT_SPEED;
}

void Player::moveLeft() {
    position += vector(heading + M_PI / 2) * Player::SIDE_SPEED;
}

void Player::moveRight() {
    position += vector(heading - M_PI / 2) * Player::SIDE_SPEED;
}

void Player::moveBack() {
    position += vector(heading) * -Player::BACK_SPEED;
}

double Player::getRotation() const {
    return heading - Player::ZERO_ANGLE;
}

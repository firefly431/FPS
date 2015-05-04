#include "Player.h"

#include <iostream>

double Player::MOVEMENT_SPEED = 0.3;
double Player::SIDE_SPEED = 0.1;
double Player::BACK_SPEED = 0.1;
double Player::ZERO_ANGLE = M_PI;
double Player::COLLISION_RADIUS = 50;

Player::Player() : position(0, 0), heading(0), input() {
    input.up = input.down = input.left = input.right = false;
}
Player::Player(const vector pos, const double h) : position(pos), heading(h) {
    input.up = input.down = input.left = input.right = false;
}

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

void Player::move(const std::vector<Line> &walls) {
	if (input.up) moveForward();
	if (input.down) moveBack();
	if (input.left) moveLeft();
	if (input.right) moveRight();
	Circle circ(getCollisionBounds());
	int colcount = 0;
	if (false) {
redo_collision:
		if (++colcount > 20) {
			std::cerr << "Over 20 collisions; aborting" << std::endl;
			return;
		}
		circ = getCollisionBounds();
	}
#ifdef _MSC_VER
	for each (const auto &wall in walls) {
#else
	for (const auto &wall : walls) {
#endif
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

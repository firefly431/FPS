#pragma once

#include <vector>
#include "vector.h"
#include "Circle.h"

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
	
	struct {
		bool up, left, right, down;
		double rotation;
	} input;

	vector position;
    double heading;

	void move(const std::vector<Line> &walls);

	Circle getCollisionBounds() const;

    static double MOVEMENT_SPEED, SIDE_SPEED, BACK_SPEED;
    static double ZERO_ANGLE;
	static double COLLISION_RADIUS;
protected:
    double getRotation() const;
	void rotate(double amount);
    void moveForward();
    void moveLeft();
    void moveRight();
    void moveBack();
};

#include "Spear.h"

#include "Line.h"
#include "Player.h"

const double Spear::SPEAR_SPEED = 2;

Spear::Spear(vector head, double direction, Player *owner) : head(head), velocity(direction), owner(owner) {
    velocity *= SPEAR_SPEED;
}

bool Spear::move(std::vector<Player> &players, const std::vector<Line> &walls) {
    auto cb = getCollisionBounds();
    // first collide with walls
    vector colpt;
    for (const Line &w : walls) {
        // process w
        if (w.intersects(cb, &colpt)) {
            return false; // destroy
        }
    }
    for (Player &p : players) {
        if (&p == owner) continue; // don't process owner
        // process p
        if (p.getCollisionBounds().intersects(cb, &colpt)) {
            p.hit(*this);
            return false; // destroy
        }
    }
    head += velocity;
    return true;
}

Line Spear::getCollisionBounds() const {
    return Line(head, head + velocity);
}

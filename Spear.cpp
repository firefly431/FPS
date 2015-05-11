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
#if defined(_MSC_VER) && _MSC_VER < 1800
    auto w_it = walls.cend();
    for (auto it = walls.cbegin(); it != w_it; it++) {
        const auto &w = *it;
#else
    for (const Line &w : walls) {
#endif
        // process w
        if (w.intersects(cb, &colpt)) {
            return false; // destroy
        }
    }
#if defined(_MSC_VER) && _MSC_VER < 1800
    auto e_it = players.end();
    for (auto it = players.begin(); it != e_it; it++) {
        auto &p = *it;
#else
    for (Player &p : players) {
#endif
        if (&p == owner) continue;
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

#include "AIController.h"

#include "Player.h"

#include <limits>

typedef MapGraph::Node *NodeRef;

NodeRef AIController::find(vector &target) {
    double mindist = std::numeric_limits<double>::infinity();
    NodeRef ret = nullptr;
#if _MSC_VER < 1800
    auto n_it = nodes.nodes.end();
    for (auto it = nodes.nodes.begin(); it != n_it; it++) {
        auto &node = *it;
#else
    for (auto &node : nodes.nodes) {
#endif
        double dist = node.position - target;
        if (dist <= node.radius && dist < mindist) {
            ret = &node;
            mindist = dist;
        }
    }
    return ret;
}

AIController::AIController(Player &target, MapGraph &nodes)
: target(target), nodes(nodes), path() {}

void AIController::update(Player &me) {
    NodeRef pos = find(me.position);
    NodeRef nt = find(target.position);
    if (nt != path.back()) {
        // try to search backward a few nodes
        auto it = path.rbegin();
        auto eit = path.rend();
        // lol
        if (it != eit) {
            if (*it == nt) {
                path.pop_back();
                goto follow_path;
            }
            if (++it != eit) {
                if (*it == nt) {
                    path.pop_back();
                    path.pop_back();
                    goto follow_path;
                }
                if (++it != eit) {
                    if (*it == nt) {
                        path.pop_back();
                        path.pop_back();
                        path.pop_back();
                        goto follow_path;
                    }
                }
            }
        }
        // maybe one of node's neighbors?
        auto nn = path.back();
#if _MSC_VER < 1800
        auto ee = nn->edges.end();
        for (auto it = nn->edges.begin(); it != ee; it++) {
            auto &edge = *it;
#else
        for (const auto &edge : nn->edges) {
#endif
            if (edge.to == nt) {
                path.push_back(nt);
                goto follow_path;
            }
        }
        path.clear();
        calculate_path(pos, nt);
    }
    // I could use a do while false loop but whatever
follow_path:
    // reminder: NodeRef pos = find(me.position);
    if (!path.empty()) {
        if (pos == path.front()) {
            path.pop_front();
            if (path.empty()) {
                shoot(me);
                return;
            }
        }
        face(me, path.front()->position);
        walk(me);
    }
    shoot(me);
}

void AIController::walk(Player &me) {
    me.input.up = true;
    me.input.left = me.input.right = me.input.down = me.input.fire = false;
}

// we're actually walking too
void AIController::shoot(Player &me) {
    me.input.up = me.input.fire = true;
    me.input.left = me.input.right = me.input.down = false;
}

void AIController::face(Player &me, const vector &target) {
    me.heading = (target - me.position).angle();
}

void AIController::calculate_path(NodeRef pos, NodeRef target) {
    //
}

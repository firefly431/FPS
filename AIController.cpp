#include "AIController.h"

#include "Player.h"

#include <limits>
#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

const double AIController::DISTANCE = 4.2;
const double AIController::TARGET_DISTANCE = 8.2;
const double AIController::BUFFER = 0.5;
const double AIController::FOV = M_PI / 4;
const double AIController::GROSS_ROTATION = 0.5;
const double AIController::GROSS_THRESHOLD = 0.8;
const double AIController::ROTATION_SPEED = 0.01;
const int AIController::REACTION_TIME = 10;

typedef MapGraph::Node *NodeRef;

NodeRef AIController::find(vector &target) {
    double mindist = std::numeric_limits<double>::infinity();
    NodeRef ret = nullptr;
#if defined(_MSC_VER) && _MSC_VER < 1800
    auto n_it = nodes.nodes.end();
    for (auto it = nodes.nodes.begin(); it != n_it; it++) {
        auto &node = *it;
#else
    for (auto &node : nodes.nodes) {
#endif
        double dist = (double)(node.position - target);
        if (dist < mindist && node.shape->contains(target)) {
            ret = &node;
            mindist = dist;
        }
    }
    return ret;
}

AIController::AIController(Player &target, MapGraph &nodes, std::vector<Line> &walls)
: target(target), nodes(nodes), walls(walls), path(), reaction_timer(REACTION_TIME) {}

void AIController::update(Player &me) {
    if ((me.position - target.position).sqr() <= TARGET_DISTANCE * TARGET_DISTANCE) {
        Line of_sight(me.position, target.position);
        bool can_see = true;
#if defined(_MSC_VER) && _MSC_VER < 1800
        auto e_it = walls.end();
        for (auto it = walls.begin(); it != e_it; it++) {
            auto &wall = *it;
#if 0
        }
#endif
#else
        for (auto &wall : walls) {
#endif
            if (of_sight.intersects(wall, nullptr)) {
                can_see = false;
                break;
            }
        }
        double adjust = std::abs((target.position - me.position).angle() - me.heading);
        if (adjust > M_PI) adjust -= M_PI * 2;
        if (can_see && std::abs(adjust) < FOV)
            shootAt(me, target.position);
        else {
            me.input.up = me.input.down = me.input.left = me.input.right = me.input.fire = false;
            me.rotate(GROSS_ROTATION);
            reaction_timer = REACTION_TIME;
        }
        return;
    }
    NodeRef pos = find(me.position);
    NodeRef nt = find(target.position);
    if (nt && nt != pos && (path.empty() || nt != path.back())) {
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
        // recalculate
        path.clear();
        calculate_path(pos, nt);
    }
    // I could use a do while false loop but whatever
follow_path:
    if (!pos) {
        me.input.down = me.input.right = true;
        me.input.up = me.input.left = me.input.fire = false;
        reaction_timer = REACTION_TIME;
        return;
    }
    // reminder: NodeRef pos = find(me.position);
    if (!path.empty()) {
        if (pos == path.front()) {
            path.pop_front();
            if (path.empty()) {
                shoot(me);
                reaction_timer = REACTION_TIME;
                return;
            }
        }
        if (turnTowards(me, path.front()->position))
            walk(me);
        reaction_timer = REACTION_TIME;
        return;
    }
    shootAt(me, target.position);
}

void AIController::walk(Player &me) {
    me.input.up = true;
    me.input.left = me.input.right = me.input.down = me.input.fire = false;
}

void AIController::shootAt(Player &me, const vector &target) {
    if (reaction_timer != 1) {
        reaction_timer--;
        return;
    }
    turnTowards(me, target);
    me.input.up = true;
    me.input.left = me.input.right = me.input.down = false;
    double dist = (target - me.position).sqr();
    if (dist < DISTANCE * DISTANCE)
        me.input.up = !(me.input.down = true);
    else if (dist < (DISTANCE + BUFFER) * (DISTANCE + BUFFER))
        me.input.up = false;
    me.input.fire = !me.input.down;
}

// we're actually walking too
void AIController::shoot(Player &me) {
    me.input.up = me.input.fire = true;
    me.input.left = me.input.right = me.input.down = false;
}

void AIController::face(Player &me, const vector &target) {
    me.heading = (target - me.position).angle();
}

bool AIController::turnTowards(Player &me, const vector &target) {
    double adjust = (target - me.position).angle() - me.heading;
    if (adjust > M_PI) adjust -= M_PI * 2;
    if (adjust < -M_PI) adjust += M_PI * 2;
    if (std::abs(adjust) < GROSS_THRESHOLD) {
        if (std::abs(adjust) < ROTATION_SPEED)
            me.rotate(adjust);
        else
            me.rotate(adjust < 0 ? -ROTATION_SPEED : ROTATION_SPEED);
    } else
        me.rotate(adjust < 0 ? -GROSS_ROTATION : GROSS_ROTATION);
    return std::abs(adjust) < ROTATION_SPEED;
}

// everything's public because no reason for encapsulation
// it's a private interface
struct PriorityQueue {
    typedef std::pair<double, NodeRef> element;
    std::vector<element> data;
    PriorityQueue(int sz) : data() {data.reserve(sz);}
    // convention: i = 1-based, I = 0-based
    inline void swap(int i, int j) {
        std::iter_swap(data.begin() + (i - 1), data.begin() + (j - 1));
    }
    inline void swapUp(int i) {
        swap(i, i / 2);
    }
    inline double get(int i) {
        return data[i - 1].first;
    }
    // STILL 1-based
    inline NodeRef operator[](int i) {
        return data[i - 1].second;
    }
    void bubble(int i) {
        while (i > 1 && get(i) < get(i / 2)) {
            swapUp(i);
            i /= 2;
        }
    }
    void heapify(int i) {
        int sz = data.size();
        int sz2 = sz / 2;
        while (i <= sz2) {
            int minc = (i * 2 == sz ? i * 2 :
                        get(i * 2) < get(i * 2 + 1) ?
                        i * 2 : i * 2 + 1);
            if (get(minc) < get(i)) {
                swap(i, minc);
                i = minc;
            } else break;
        }
    }
    void insert(NodeRef el, double p) {
#if defined(_MSC_VER) && _MSC_VER < 1800
        data.push_back(std::make_pair(p, el));
#else
        data.emplace_back(p, el);
#endif
        bubble(data.size());
    }
    void remove() {
        swap(1, data.size());
        data.pop_back();
        heapify(1);
    }
    void decrease(int i, double p) {
        data[i - 1].first = p;
        bubble(i);
    }
    // 1 based, returns 0 on not found
    int search(NodeRef target) {
        for (int i = data.size(); i >= 1; --i) {
            if ((*this)[i] == target) return i;
        }
        return 0;
    }
};

inline static double heuristic(NodeRef a, NodeRef b) {
    double ret = (double)(a->position - b->position);
    return ret;
}

void AIController::calculate_path(NodeRef pos, NodeRef target) {
    if (!pos) return;
    PriorityQueue Q(nodes.nodes.size());
    Q.insert(pos, 0);
    std::unordered_map<NodeRef, NodeRef> prev;
    std::unordered_map<NodeRef, double> cost;
    prev[pos] = pos;
    cost[pos] = 0;
    while (!Q.data.empty()) {
        NodeRef cur = Q.data.front().second;
        if (cur == target) {
            // reconstruct the path
            // path is already cleared for us
            path.push_front(target);
            while (target != pos) {
                target = prev[target];
                path.push_front(target);
            }
            return;
        }
        Q.remove();
#if defined(_MSC_VER) && _MSC_VER < 1800
        auto e_it = cur->edges.end();
        for (auto it = cur->edges.begin(); it != e_it; it++) {
            auto &edge = *it;
#if 0
        }
#endif
#else
        for (auto &edge : cur->edges) {
#endif
            NodeRef next = edge.to;
            double ncost = cost[cur] + edge.weight;
            int i = Q.search(next);
            if (!cost.count(next) || ncost < cost[next]) {
                double hcost = ncost + heuristic(next, target);
                cost[next] = ncost;
                prev[next] = cur;
                if (i) {
                    if (hcost < Q.get(i))
                        Q.decrease(i, hcost);
                } else
                    Q.insert(next, hcost);
            }
        }
    }
}

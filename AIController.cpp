#include "AIController.h"

#include "Player.h"

#include <limits>
#include <utility>
#include <algorithm>
#include <vector>

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

// everything's public because no reason for encapsulation
// it's a private interface
struct PriorityQueue {
    typedef std::pair<double, NodeRef> element;
    std::vector<element> data;
    PriorityQueue(int sz) : data(sz) {}
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
            }
        }
    }
    void insert(NodeRef el, double p) {
#if _MSC_VER < 1800
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

void AIController::calculate_path(NodeRef pos, NodeRef target) {
    //
}

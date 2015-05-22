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

// get the Node in the Graph for the target position
NodeRef AIController::find(vector &target) {
    // find the node with the smallest distance and that contains the position
    double mindist = std::numeric_limits<double>::infinity();
    NodeRef ret = nullptr;
    for (auto &node : nodes.nodes) {
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
    // if we're close to the target
    if ((me.position - target.position).sqr() <= TARGET_DISTANCE * TARGET_DISTANCE) {
        Line of_sight(me.position, target.position);
        // can we see the target?
        bool can_see = true;
        // go through the wall and check
        for (auto &wall : walls) {
            if (of_sight.intersects(wall, nullptr)) {
                can_see = false;
                break;
            }
        }
        // angle difference
        double adjust = std::abs((target.position - me.position).angle() - me.heading);
        if (adjust > M_PI) adjust -= M_PI * 2;
        // if we can see him and he's inside our field of view
        if (can_see && std::abs(adjust) < FOV)
            shootAt(me, target.position);
        else {
            // do nothing
            me.input.up = me.input.down = me.input.left = me.input.right = me.input.fire = false;
            // and look around
            me.rotate(GROSS_ROTATION);
            reaction_timer = REACTION_TIME;
        }
        return;
    }
    // else, pathfinding mode
    // our position
    NodeRef pos = find(me.position);
    // target's position (Node of Target)
    NodeRef nt = find(target.position);
    // if the target has moved (or we don't have a path)
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
    // I could use a do while false loop but IMHO this is clearer
follow_path:
    // if we're lost (this happens very rarely)
    if (!pos) {
        // go right and backwards (pretty arbitrary)
        me.input.down = me.input.right = true;
        me.input.up = me.input.left = me.input.fire = false;
        reaction_timer = REACTION_TIME;
        return;
    }
    // reminder: NodeRef pos = find(me.position);
    if (!path.empty()) {
        // follow the path
        // if we've reached the next node in the path
        if (pos == path.front()) {
            path.pop_front();
            if (path.empty()) { // if we just finished, just shoot
                shoot(me);
                reaction_timer = REACTION_TIME;
                return;
            }
        }
        // turn towards the next node but only walk if we're oriented correctly
        if (turnTowards(me, path.front()->position))
            walk(me);
        reaction_timer = REACTION_TIME;
        return;
    }
    shootAt(me, target.position);
}

// set of inputs to walk
void AIController::walk(Player &me) {
    me.input.up = true;
    me.input.left = me.input.right = me.input.down = me.input.fire = false;
}

// set of inputs to shoot at the player
void AIController::shootAt(Player &me, const vector &target) {
    // we have "reflexes" so only do stuff if some time has passed
    if (reaction_timer != 1) {
        reaction_timer--;
        return;
    }
    turnTowards(me, target);
    // walk
    me.input.up = true;
    me.input.left = me.input.right = me.input.down = false;
    // square of the distance between target and me (little optimization)
    double dist = (target - me.position).sqr();
    if (dist < DISTANCE * DISTANCE)
        me.input.up = !(me.input.down = true); // go backwards and not forwards
    else if (dist < (DISTANCE + BUFFER) * (DISTANCE + BUFFER)) // if we're at a comfortable position
        me.input.up = false;
    me.input.fire = !me.input.down; // fire when not retreating
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
    // if the angle is small
    if (std::abs(adjust) < GROSS_THRESHOLD) {
        // if we can just rotate
        if (std::abs(adjust) < ROTATION_SPEED)
            me.rotate(adjust);
        else // rotate with correct direction
            me.rotate(adjust < 0 ? -ROTATION_SPEED : ROTATION_SPEED);
    } else // rotate with a large rotation
        me.rotate(adjust < 0 ? -GROSS_ROTATION : GROSS_ROTATION);
    // if angle is very close
    return std::abs(adjust) < ROTATION_SPEED;
}

// everything's public because no reason for encapsulation
// it's a private interface
struct PriorityQueue {
    typedef std::pair<double, NodeRef> element;
    // typical ordering of a binary heap
    // properties:
    // with one-based i:
    // i's first child is i * 2
    // i's second child is i * 2 + 1
    // i's parent is i / 2 (rounding down)
    // results from property that binary heaps are always complete
    std::vector<element> data;
    PriorityQueue(int sz) : data() {data.reserve(sz);}
    // convention: i = 1-based, I = 0-based
    // swap two elemnts
    inline void swap(int i, int j) {
        std::iter_swap(data.begin() + (i - 1), data.begin() + (j - 1));
    }
    // swap an element up the tree
    inline void swapUp(int i) {
        swap(i, i / 2);
    }
    // get element at i's distance
    inline double get(int i) {
        return data[i - 1].first;
    }
    // STILL 1-based
    // get the node not the distance
    inline NodeRef operator[](int i) {
        return data[i - 1].second;
    }
    // bubble up (used when adding and removing)
    void bubble(int i) {
        while (i > 1 && get(i) < get(i / 2)) {
            swapUp(i);
            i /= 2;
        }
    }
    // heapify down (used when removing)
    void heapify(int i) {
        int sz = data.size();
        int sz2 = sz / 2; // size/2 (parent of last element)
        while (i <= sz2) {
            // explanation:
            // minc is the index of the smaller child
            // if i is the SOLE parent of the last element, then minc must be just the child
            // otherwise it's just the smaller child
            int minc = (i * 2 == sz ? i * 2 :
                        get(i * 2) < get(i * 2 + 1) ?
                        i * 2 : i * 2 + 1);
            if (get(minc) < get(i)) { // if child is smaller than us (violation of heap property)
                swap(i, minc); // swap down
                i = minc; // and set the current node
            } else break;
        }
    }
    // insert an element
    void insert(NodeRef el, double p) {
        data.emplace_back(p, el);
        // bubble up to restore heap property
        bubble(data.size());
    }
    // remove an element (min element)
    void remove() {
        // swap with end
        swap(1, data.size());
        // remove it
        data.pop_back();
        // and heapify entire heap
        heapify(1);
    }
    // decrease key
    void decrease(int i, double p) {
        // set the key
        data[i - 1].first = p;
        // bubble up
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

// heuristic used for A*
inline static double heuristic(NodeRef a, NodeRef b) {
    // just Euclidean distance
    double ret = (double)(a->position - b->position);
    return ret;
}

// calculate the path (stores in member)
// this is exactly A*
// note that a precondition of Dijkstra (and therefore A*) is that there must be no negative weights
void AIController::calculate_path(NodeRef pos, NodeRef target) {
    if (!pos) return;
    PriorityQueue Q(nodes.nodes.size());
    Q.insert(pos, 0);
    // previous node in the min path
    std::unordered_map<NodeRef, NodeRef> prev;
    // cost to go to this node based on nodes considered
    std::unordered_map<NodeRef, double> cost;
    // previous node of start is start
    prev[pos] = pos;
    // cost is 0
    cost[pos] = 0;
    while (!Q.data.empty()) {
        // iterate through the essentially linked list and add to path
        NodeRef cur = Q.data.front().second;
        if (cur == target) {
            // reconstruct the path
            // path is already cleared for us
            // so we don't need to clear it again
            path.push_front(target);
            while (target != pos) {
                target = prev[target];
                path.push_front(target);
            }
            return;
        }
        // remove an element to consider
        Q.remove();
        // go through the edges
        for (auto &edge : cur->edges) {
            // get the node to consider
            NodeRef next = edge.to;
            // and calculate the cost based on its weight
            double ncost = cost[cur] + edge.weight;
            // if we haven't considered it yet
            // or if the new cost is less than the one we considered previously
            if (!cost.count(next) || ncost < cost[next]) {
                // calculate the heuristic cost (exact cost plus heuristic to target)
                double hcost = ncost + heuristic(next, target);
                // update our maps
                cost[next] = ncost;
                prev[next] = cur;
                // add it to our priority queue
                Q.insert(next, hcost);
            }
        }
    }
}

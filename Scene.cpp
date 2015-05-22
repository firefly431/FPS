#include "Scene.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "Quad.h"

// this stuff is to make the pairs able to put in unordered_set containers without ordering

typedef std::pair<vector *, vector *> pvec_;

struct pair_hasher {
    inline std::size_t operator()(const pvec_ &v) const {
        std::hash<vector *> inthasher;
        // simple order-independent hash
        return (inthasher(v.first) ^ inthasher(v.second)) + inthasher((vector *)((size_t)v.first + (size_t)v.second));
    }
};

struct pair_equal {
    inline bool operator()(const pvec_ &a, const pvec_ &b) const {
        return (a.first == b.first && a.second == b.second) ||
               (a.second == b.first && a.first == b.second);
    }
};

void Scene::loadGraph(const std::string &fname) {
    std::ifstream fin(fname);
    fin >> std::skipws;
    std::string type, line;
    std::vector<vector> vertices;
    std::unordered_set<std::pair<vector *, vector *>,
                       pair_hasher, pair_equal> walls;
    // temp variables
    float x, y;
    int a, b, c, d;
    // keep reading lines
    // similar to OBJ reader
    while (fin.good()) {
        std::getline(fin, line);
        std::istringstream in(line);
        if (line == "") continue;
        in >> type;
        if (strcasecmp(type.c_str(), "v") == 0) {
            in >> x; in >> y; // ignore z
            vertices.emplace_back(x, y);
        } else if (strcasecmp(type.c_str(), "f") == 0) {
            MapGraph::Node *node = nullptr; // stores the node
            in >> a; in >> b; in >> c; in >> std::ws;
            vector &A = vertices[a - 1],
                   &B = vertices[b - 1],
                   &C = vertices[c - 1];
            vector *D = nullptr;
            vector *line[4]; // hold the line created by edges
            vector **lptr;
            if (isdigit(in.peek())) {
                in >> d;
                D = &vertices[d - 1];
                node = graph.addNode((A + B + C + *D) / 4.,
                                     new Quad(A, B, C, *D));
                // add edges to walls
                walls.emplace(&A, &B);
                walls.emplace(&B, &C);
                walls.emplace(&C, D);
                walls.emplace(D, &A);
            } else {
                node = graph.addNode((A + B + C) / 3.,
                                     new Triangle(A, B, C));
                walls.emplace(&A, &B);
                walls.emplace(&B, &C);
                walls.emplace(&C, &A);
            }
            // kind of slow (O(N^2))
            // if another poly shares two points, and
            // the player is able to pass through the
            // line, add an edge
            for (auto &n : graph.nodes) {
                if (&n == node) continue;
                lptr = line; // see the declaration above
                // hold the points in common
                if (n.shape->hasPoint(A))
                    *(lptr++) = &A;
                if (n.shape->hasPoint(B))
                    *(lptr++) = &B;
                if (n.shape->hasPoint(C))
                    *(lptr++) = &C;
                if (D && n.shape->hasPoint(*D))
                    *(lptr++) = D;
                if (lptr >= line + 2) { // lptr - line >= 2 (distance >= 2)
                    // should always = 2/1/0 for good graphs
                    walls.erase(std::make_pair(line[0], line[1]));
                    // if the line is sufficiently long for the player to pass through
                    // then add an edge between the edges
                    if ((*line[1] - *line[0]).sqr() > 4 * Player::COLLISION_RADIUS * Player::COLLISION_RADIUS) {
                        n.addEdge(node);
                        node->addEdge(&n);
                    }
                }
            }
        }
    }
    // generate walls
    for (auto &p : walls) {
        this->walls.emplace_back(*p.first, *p.second);
    }
    // generate list of islands
    while (!walls.empty()) {
        // get an island
        std::list<vector> island;
        // the sharpest angle has the highest cosine
        // and we rotate the island so the sharpest angle
        // has the seam
        // note: dot product is |A| |B| cos \theta
        // where \theta is the angle between them
        double maxdot = -2.0;
        // iterator to vertex with largest dot product
        auto maxit = island.begin();
        // current
        auto it = walls.begin();
        // end iterator (no need to compute each time)
        auto e_it = walls.end();
        auto edge = *it;
        walls.erase(it); // remove the first element
        auto start = edge.first, pt = edge.second; // get the start vertex and the endpoint (this is the current point)
        island.push_back(*start); // add to island
        // basically go around the island searching for the current point
        while (!walls.empty() && pt != start) {
            // add point
            island.push_back(*pt);
            // find wall with that point...
            it = walls.begin();
            e_it = walls.end(); // it seems that end()
                                // iterator is not invalidated
                                // but just to be safe
            while (it != e_it) {
                if (it->first == pt) {
                    pt = it->second; // ...and set the point to the next one
                    break;
                } else if (it->second == pt) {
                    pt = it->first;
                    break;
                }
                it++;
            }
            walls.erase(it); // remove that edge
            // calculate cos of angle
            auto it2 = island.rbegin(); // get the last three points
            auto p1 = *(it2++);
            auto p2 = *(it2++);
            auto p3 = *it2;
            double dot = (p1 - p2).normalized() * // and calculate the cosine
                         (p3 - p2).normalized();
            if (dot > maxdot) { // then set the max
                maxdot = dot;
                auto itt = ++island.rbegin(); // get the second from last
                ++it;
                maxit = itt.base();
            }
        }
        // rotate the island
        std::rotate(island.begin(), maxit, island.end());
        // put the first vertex back (closed loop)
        island.push_back(island.front());
        islands.push_back(island);
    }
}

bool Scene::wouldCollide(const vector &v) {
    for (auto &p : players) {
        if ((p.position - v).sqr() < Player::COLLISION_RADIUS * Player::COLLISION_RADIUS) return true;
    }
    return false;
}

vector Scene::addPlayer() {
    vector ret;
    // get the first node that isn't taken
    for (auto &n : graph.nodes) {
        if (!wouldCollide(n.position)) {
            ret = n.position;
            break;
        }
    }
    players.emplace_back(ret, 0);
    return ret;
}

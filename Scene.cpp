#include "Scene.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "Quad.h"
#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

typedef std::pair<vector *, vector *> pvec_;

struct pair_hasher {
    inline std::size_t operator()(const pvec_ &v) const {
        std::hash<vector *> inthasher;
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
    float x, y;
    int a, b, c, d;
    while (fin.good()) {
        std::getline(fin, line);
        std::istringstream in(line);
        if (line == "") continue;
        in >> type;
        if (strcasecmp(type.c_str(), "v") == 0) {
            in >> x; in >> y; // ignore z
#if _MSC_VER < 1800
            vertices.push_back(vector(x, y));
#else
            vertices.emplace_back(x, y);
#endif
        } else if (strcasecmp(type.c_str(), "f") == 0) {
            MapGraph::Node *node = nullptr;
            in >> a; in >> b; in >> c; in >> std::ws;
            vector &A = vertices[a - 1],
                   &B = vertices[b - 1],
                   &C = vertices[c - 1];
            vector *D = nullptr;
            vector *line[4];
            vector **lptr;
            if (isdigit(in.peek())) {
                in >> d;
                D = &vertices[d - 1];
                // because VS2010 doesn't support
                // explicit conversion (WHY NOT)
                node = graph.addNode((A + B + C + *D) / 4.,
                                     new Quad(A, B, C, *D));
                // add edges to walls
#if defined(_MSC_VER) && _MSC_VER < 1800
                walls.insert(std::make_pair(&A, &B));
                walls.insert(std::make_pair(&B, &C));
                walls.insert(std::make_pair(&C, D));
                walls.insert(std::make_pair(D, &A));
#else
                walls.emplace(&A, &B);
                walls.emplace(&B, &C);
                walls.emplace(&C, D);
                walls.emplace(D, &A);
#endif
            } else {
                node = graph.addNode((A + B + C) / 3.,
                                     new Triangle(A, B, C));
#if defined(_MSC_VER) && _MSC_VER < 1800
                walls.insert(std::make_pair(&A, &B));
                walls.insert(std::make_pair(&B, &C));
                walls.insert(std::make_pair(&C, &A));
#else
                walls.emplace(&A, &B);
                walls.emplace(&B, &C);
                walls.emplace(&C, &A);
#endif
            }
            // kind of slow
            // if another poly shares two points, and
            // the player is able to pass through the
            // line, add an edge
#if _MSC_VER < 1800
            auto c_it = graph.nodes.end();
            for (auto it = graph.nodes.begin(); it != c_it; it++) {
                auto &n = *it;
#if 0
            }
#endif
#else
            for (auto &n : graph.nodes) {
#endif
                if (&n == node) continue;
                lptr = line;
                if (n.shape->hasPoint(A))
                    *(lptr++) = &A;
                if (n.shape->hasPoint(B))
                    *(lptr++) = &B;
                if (n.shape->hasPoint(C))
                    *(lptr++) = &C;
                if (D && n.shape->hasPoint(*D))
                    *(lptr++) = D;
                if (lptr >= line + 2) {
                    walls.erase(std::make_pair(line[0], line[1]));
                    if ((*line[1] - *line[0]).sqr() > 4 * Player::COLLISION_RADIUS * Player::COLLISION_RADIUS) {
                        n.addEdge(node);
                        node->addEdge(&n);
                    }
                }
            }
        }
    }
    // generate walls
#if defined(_MSC_VER) && _MSC_VER < 1800
    auto e_it = walls.end();
    for (auto it = walls.begin(); it != e_it; it++) {
        auto &p = *it;
        this->walls.push_back(Line(*p.first, *p.second));
#if 0
    }
#endif
#else
    for (auto &p : walls) {
        this->walls.emplace_back(*p.first, *p.second);
#endif
    }
    while (!walls.empty()) {
        // get an island
        std::list<vector> island;
        // the sharpest angle has the highest cosine
        // and we rotate the island so the sharpest angle
        // has the seam
        double maxdot = -2.0;
        auto maxit = island.begin();
        auto it = walls.begin();
        auto e_it = walls.end();
        auto edge = *it;
        walls.erase(it);
        auto start = edge.first, pt = edge.second;
        island.push_back(*start);
        while (!walls.empty() && pt != start) {
            island.push_back(*pt);
            it = walls.begin();
            e_it = walls.end(); // it seems that end()
                                // iterator is not invalidated
                                // but just to be safe
            while (it != e_it) {
                if (it->first == pt) {
                    pt = it->second;
                    break;
                } else if (it->second == pt) {
                    pt = it->first;
                    break;
                }
                it++;
            }
            walls.erase(it);
            // calculate cos of angle
            auto it2 = island.rbegin();
            auto p1 = *(it2++);
            auto p2 = *(it2++);
            auto p3 = *it2;
            double dot = (p1 - p2).normalized() *
                         (p3 - p2).normalized();
            if (dot > maxdot) {
                maxdot = dot;
                auto itt = ++island.rbegin();
                ++it;
                maxit = itt.base();
            }
        }
        std::rotate(island.begin(), maxit, island.end());
        island.push_back(island.front());
#if defined(_MSC_VER) && _MSC_VER < 1800
        islands.emplace_back(island);
#else
        islands.push_back(island);
#endif
    }
}

bool Scene::wouldCollide(const vector &v) {
#if _MSC_VER < 1800
    auto p_it = players.end();
    for (auto it = players.begin(); it != p_it; it++) {
        auto &p = *it;
#if 0
    }
#endif
#else
    for (auto &p : players) {
#endif
        if ((p.position - v).sqr() < Player::COLLISION_RADIUS * Player::COLLISION_RADIUS) return true;
    }
    return false;
}

vector Scene::addPlayer() {
    vector ret;
#if _MSC_VER < 1800
    auto c_it = graph.nodes.end();
    for (auto it = graph.nodes.begin(); it != c_it; it++) {
        auto &n = *it;
#if 0
    }
#endif
#else
    for (auto &n : graph.nodes) {
#endif
        if (!wouldCollide(n.position)) {
            ret = n.position;
            break;
        }
    }
#if _MSC_VER < 1800
    players.push_back(Player(ret, 0));
#else
    players.emplace_back(ret, 0);
#endif
    return ret;
}

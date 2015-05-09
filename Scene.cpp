#include "Scene.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "Quad.h"
#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

void Scene::loadWalls(const std::string &fname) {
    std::ifstream fin(fname);
    fin >> std::skipws;
    std::string type, line;
    std::vector<vector> vertices;
    float x, y;
    int a, b;
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
        } else if (strcasecmp(type.c_str(), "l") == 0) {
            in >> a; in >> b;
#if _MSC_VER < 1800
            walls.push_back(Line(vertices[a - 1], vertices[b - 1]));
#else
            walls.emplace_back(vertices[a - 1], vertices[b - 1]);
#endif
        }
    }
}

void Scene::loadGraph(const std::string &fname) {
    std::ifstream fin(fname);
    fin >> std::skipws;
    std::string type, line;
    std::vector<vector> vertices;
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
                std::cout << "Added quad" << std::endl;
            } else {
                node = graph.addNode((A + B + C) / 3.,
                                     new Triangle(A, B, C));
                std::cout << "Added tri" << std::endl;
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
                    if ((*line[1] - *line[0]).sqr() > 4 * Player::COLLISION_RADIUS * Player::COLLISION_RADIUS) {
                        n.addEdge(node);
                        node->addEdge(&n);
                    }
                }
            }
        }
    }
}

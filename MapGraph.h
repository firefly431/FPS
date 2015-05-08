#pragma once

#include "vector.h"

#include <list>

class MapGraph {
public:
    MapGraph();

    struct Node;

    struct Edge {
        double weight;
        Node *to;
    };

    struct Node {
        Node(vector position, double radius);
        vector position;
        double radius;
        std::list<Edge> edges;
        void addEdge(Node *to);
    };

    Node *addNode(vector position, double radius);

    std::list<Node> nodes;
};
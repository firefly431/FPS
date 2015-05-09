#pragma once

#include "vector.h"
#include "Shape.h"

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
        Node(vector position, Shape *shape);
        Node(Node &&move);
        vector position;
        std::unique_ptr<Shape> shape;
        std::list<Edge> edges;
        void addEdge(Node *to);
    };

    // will take ownership
    Node *addNode(vector position, Shape *shape);

    std::list<Node> nodes;
};

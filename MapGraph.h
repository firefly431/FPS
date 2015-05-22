#pragma once

#include "vector.h"
#include "Shape.h"

#include <list>

// graph of map nodes (for pathfinding)
class MapGraph {
public:
    MapGraph();

    // forward declaration
    struct Node;

    struct Edge {
        double weight;
        Node *to;
    };

    struct Node {
        Node(vector position, Shape *shape);
        Node(Node &&move); // move ctor
        vector position;
        std::unique_ptr<Shape> shape;
        std::list<Edge> edges;
        void addEdge(Node *to);
    };

    // will take ownership
    Node *addNode(vector position, Shape *shape);

    std::list<Node> nodes;
};

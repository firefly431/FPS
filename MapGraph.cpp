#include "MapGraph.h"

MapGraph::MapGraph() : nodes() {}

MapGraph::Node::Node(vector position, double radius) : position(position), radius(radius), edges() {}

void MapGraph::Node::addEdge(Node *to) {
    Edge e;
    e.to = to;
    e.weight = to->position - position;
}

MapGraph::Node *MapGraph::addNode(vector position, double radius) {
    Node n(position, radius);
    nodes.push_back(n);
    return &nodes.back();
}

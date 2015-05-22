#include "MapGraph.h"

MapGraph::MapGraph() : nodes() {}

MapGraph::Node::Node(vector position, Shape *shape) : position(position), shape(shape), edges() {}

MapGraph::Node::Node(Node &&move) : position(std::move(move.position)), shape(std::move(move.shape)), edges(std::move(move.edges)) {}

void MapGraph::Node::addEdge(Node *to) {
    Edge e;
    e.to = to;
    // calculate weight
    e.weight = (double)(to->position - position);
    edges.push_back(e);
}

MapGraph::Node *MapGraph::addNode(vector position, Shape *shape) {
    nodes.emplace_back(position, shape);
    return &nodes.back();
}

#include "MapGraph.h"

MapGraph::MapGraph() : nodes() {}

MapGraph::Node::Node(vector position, Shape &&shape) : position(position), shape(&shape), edges() {}

MapGraph::Node::Node(Node &&move) : position(std::move(move.position)), shape(std::move(move.shape)), edges(std::move(move.edges)) {}

void MapGraph::Node::addEdge(Node *to) {
    Edge e;
    e.to = to;
    e.weight = to->position - position;
}

MapGraph::Node *MapGraph::addNode(vector position, Shape &&shape) {
#if _MSC_VER < 1800
    nodes.emplace_back(Node(position, std::move(shape)));
#else
    nodes.emplace_back(position, std::move(shape));
#endif
    return &nodes.back();
}

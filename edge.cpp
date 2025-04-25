#include <string>

#include "edge.h"

Edge::Edge() {}

/** constructor with label and weight */
Edge::Edge(const std::string& end, int weight)
{
    edgeWeight = weight;
    endVertex = end;
}

/** return the vertex this edge connects to */
std::string Edge::getEndVertex() const { return endVertex; }

/** return the weight/cost of travlleing via this edge */
int Edge::getWeight() const { return edgeWeight; }

/**
 * Used by vertex to keep track of all the vertices connects to
 * Each edge has a weight, possibly 0
 * Edge is a simple container class, no interesting functions
 */

#ifndef EDGE_H
#define EDGE_H

#include <string>

class Edge {
 public:
    /** empty */
    Edge();

    /** constructor with label and weight */
    Edge(const std::string& end, int weight);

    /** return the vertex this edge connects to */
    std::string getEndVertex() const;

    /** return the weight/cost of travlleing via this edge */
    int getWeight() const;

 private:
    /** end vertex, cannot be changed */
    std::string endVertex {""};

    /** edge weight, cannot be changed */
    int edgeWeight {0};
};  //  end Edge


#endif  // EDGE_H

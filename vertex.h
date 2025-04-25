/**
 * Each vertex has a unique label
 * Can be connected to other vertices via weighted edges
 * Cannot be connected to itself
 * Used by depth-first search and breadth-first search
 * to keep track of if it has been visited or not
 */

#ifndef VERTEX_H
#define VERTEX_H

#include <functional>
#include <map>
#include <string>

#include "edge.h"

class Vertex {
 public:
    /** Creates an unvisited vertex, gives it a label, and clears its
        adjacency list.
        NOTE: A vertex must have a unique label that cannot be changed. */
    explicit Vertex(std::string label);

    /** @return  The label of this vertex. */
    std::string getLabel() const;

    /** Marks this vertex as visited. */
    void visit();

    /** Marks this vertex as not visited. */
    void unvisit();

    /** Returns the visited status of this vertex.
     @return  True if the vertex has been visited, otherwise
        returns false/ */
    bool isVisited() const;

    /** Adds an edge between this vertex and the given vertex.
        Cannot have multiple connections to the same endVertex
        Cannot connect back to itself
     @return  True if the connection is successful. */
    bool connect(const std::string& endVertex, const int edgeWeight = 0);

    /** Removes the edge between this vertex and the given one.
    @return  True if the removal is successful. */
    bool disconnect(const std::string& endVertex);

    /** Gets the weight of the edge between this vertex and the given vertex.
     @return  The edge weight. This value is zero for an unweighted graph and
        is negative if the .edge does not exist */
    int getEdgeWeight(const std::string& endVertex) const;

    /** Calculates how many neighbors this vertex has.
     @return  The number of the vertex's neighbors. */
    int getNumberOfNeighbors() const;

    /** Sets current neighbor to first in adjacency list. */
    void resetNeighbor();

    /** Gets this vertex's next neighbor in the adjacency list.
        Neighbors are automatically sorted alphabetically via map
        Returns the vertex label if there are no more neighbors
     @return  The label of the vertex's next neighbor. */
    std::string getNextNeighbor();

    /** Sees whether this vertex is equal to another one.
        Two vertices are equal if they have the same label. */
    bool operator==(const Vertex& rightHandItem) const;

    /** Sees whether this vertex is < another one.
        Compares vertexLabel. */
    bool operator<(const Vertex& rightHandItem) const;

 private:
    /** the unique label for the vertex */
    std::string vertexLabel;

    /** True if the vertex is visited */
    bool visited {false};

    /** adjacencyList as an ordered map, in alphabetical order */
    std::map<std::string, Edge, std::less<std::string>> adjacencyList;

    /** iterator showing which neighbor we are currently at */
    std::map<std::string, Edge>::iterator currentNeighbor;
};

#endif  // VERTEX_H

#include <climits>

#include "vertex.h"


#include <functional>
#include <map>
#include <string>

#include "edge.h"

/** Creates an unvisited vertex, gives it a label, and clears its
    adjacency list.
    NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label)
{
    vertexLabel = label;
    if(adjacencyList.size() > 0) adjacencyList.clear();
    currentNeighbor = adjacencyList.begin();
}

/** @return  The label of this vertex. */
std::string Vertex::getLabel() const { return vertexLabel; }

/** Marks this vertex as visited. */
void Vertex::visit() {visited = true;}

/** Marks this vertex as not visited. */
void Vertex::unvisit() {visited = false;}

/** Returns the visited status of this vertex.
 @return  True if the vertex has been visited, otherwise
    returns false/ */
bool Vertex::isVisited() const { return visited;}

/** Adds an edge between this vertex and the given vertex.
    Cannot have multiple connections to the same endVertex
    Cannot connect back to itself
 @return  True if the connection is successful. */
bool Vertex::connect(const std::string& endVertex, const int edgeWeight)
{ 
    if(endVertex == vertexLabel) return false;
    //if new endVertex isn't neighbor then add and return true
    if(adjacencyList.find(endVertex) == adjacencyList.end())
    {
        adjacencyList.insert({endVertex, Edge(endVertex, edgeWeight)});
        return true; 
    }
    else
    {
        return false;
    }    
}

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex)
{ 
    if(endVertex == vertexLabel) return false;
    //if new endVertex is a neighbor then erase it and return true
    auto it = adjacencyList.find(endVertex);
    if(it != adjacencyList.end())
    {
        adjacencyList.erase(it);
        return true; 
    }
    else
    {
        return false;
    }    
}

/** Gets the weight of the edge between this vertex and the given vertex.
 @return  The edge weight. This value is zero for an unweighted graph and
    is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex) const
{ 
    //find edge weight to given vertex and return it, or -1 if not exist
    auto it = adjacencyList.find(endVertex);

    if(it != adjacencyList.end())
    {
        return it->second.getWeight(); 
    }
    else
    {
        return -1;
    }
      
}

/** Calculates how many neighbors this vertex has.
 @return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() const
{ 
    return adjacencyList.size();
}

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor()
{
    currentNeighbor = adjacencyList.begin();
}

/** Gets this vertex's next neighbor in the adjacency list.
    Neighbors are automatically sorted alphabetically via map
    Returns the vertex label if there are no more neighbors
 @return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor()
{ 
    auto it = currentNeighbor;
    if(currentNeighbor != adjacencyList.end()) ++currentNeighbor;
    else return vertexLabel;

    return it->first;
}

/** Sees whether this vertex is equal to another one.
    Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex& rightHandItem) const
{
    return vertexLabel == rightHandItem.getLabel();
}

/** Sees whether this vertex is < another one.
    Compares vertexLabel. */
bool Vertex::operator<(const Vertex& rightHandItem) const
{
    return vertexLabel < rightHandItem.getLabel();
}


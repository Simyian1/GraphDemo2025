#include <queue>
#include <stack>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "graph.h"

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */

/** constructor, empty graph */
Graph::Graph()
{
    numberOfEdges = 0;
    numberOfVertices = 0;
}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph()
{
    for(auto& pair : vertices)
    {
        delete pair.second;
    }
    vertices.clear();
}

/** return number of vertices */
int Graph::getNumVertices() const
{
    return numberOfVertices;
}

/** return number of edges */
int Graph::getNumEdges() const
{
    return numberOfEdges;
}

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight)
{
    if(start == end) return false;
    //if vertex is found and connect works then return true
    Vertex* vert = findOrCreateVertex(start);
    findOrCreateVertex(end);
    if(vert->connect(end, edgeWeight))
    {
        numberOfEdges++;
        return true;
    }
    else
    {
        return false;
    }
    
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const
{
    auto it = vertices.find(start);
    //found start
    if(it != vertices.end())
    {
        //if there exists an edge between start and end return it,
        //otherwise int_max
        int weight = it->second->getEdgeWeight(end);
        if(weight != -1) return weight;
    }
    return INT_MAX;
}

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename)
{
    //open file
    std::ifstream inputF(filename);

    if(inputF.is_open())
    {
        //if file is open then get the num edges and process each line
        std::string line;
        std::getline(inputF, line);
        int numEdges = std::stoi(line);
        //process each line
        while(getline(inputF, line))
        {
            //for each line extract start, end, and weight
            std::istringstream iss(line);
            std::string start, end, sWeight;
            if (iss >> start >> end >> sWeight)
            {
                //error handling for stoi
                try
                {
                     //convert string to int
                    int weight = stoi(sWeight);
                     //ad edge from start to end with weight
                    add(start, end, weight);
                }
                catch (...)
                { 
                    std::cout << "Invalid line: " << line << std::endl; 
                }
            } 
            else
            {
                std::cout << "Malformed line: " << line << '\n';
            }
        }
        if(numEdges != numberOfEdges) std::cout << "Error Read File\n";
    }
    else
    {
        std::cout << "Bad file name.\n";
    }
    
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel, 
                                void visit(const std::string&))
    {
        //unvisit
        unvisitVertices();
        if(vertices[startLabel] == nullptr) return;
        //create stack and push start label, mark it visited
        std::stack<std::string> st;
        st.push(startLabel);
        vertices[startLabel]->visit();
        //while not empty continue traversal
        while(!st.empty())
        {
            std::string top = st.top();
            st.pop();
            //call visit func on top
            visit(top);

            vertices[top]->resetNeighbor();
            //for each neighbor in the top vertex add to stack in reverse order
            //using stack
            std::stack<std::string> neiSt;
            std::string neighbor = vertices[top]->getNextNeighbor();
            //if neighbor is not visited and while list exists add to neiSt
            while(neighbor != top)
            {
                if(!vertices[neighbor]->isVisited())
                {
                    neiSt.push(neighbor);
                }
                neighbor = vertices[top]->getNextNeighbor();
            }

            //then empty neighborStack into main stack so that the first node is
            //on top
            while(!neiSt.empty())
            {
                st.push(neiSt.top());
                //visit every added vertex
                vertices[neiSt.top()]->visit();
                neiSt.pop();
            }
            //now you have the correct stack
        }
    }

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string&))
    {
        //unvisit
        unvisitVertices();
        if(vertices[startLabel] == nullptr) return;
        //create queue and push start label, mark it visited
        std::queue<std::string> q;
        q.push(startLabel);
        vertices[startLabel]->visit();
        //while not empty continue traversal
        while(!q.empty())
        {
            std::string front = q.front();
            q.pop();
            //call visit func on top
            visit(front);

            vertices[front]->resetNeighbor();
            std::string neighbor = vertices[front]->getNextNeighbor();

            //if neighbor is not visited and while list exists add to neiSt
            while(neighbor != front)
            {
                if(!vertices[neighbor]->isVisited())
                {
                    vertices[neighbor]->visit();
                    q.push(neighbor);
                }
                neighbor = vertices[front]->getNextNeighbor();
            }
            
            //now you have the correct queue
        }
    }

/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
    std::string startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous)
    {
    //unvisit
    unvisitVertices();
    if(vertices[startLabel] == nullptr) return;
    //set weights to max
    for(auto& v : vertices)
    {
        weight[v.first] = INT_MAX;
    }
    //set start weight to 0
    weight[startLabel] = 0;
    //max pq of weight and vertextlabel 
    std::priority_queue<std::pair<int, std::string>,
    std::vector<std::pair<int, std::string>>, std::greater<>> pq;
    //push it
    pq.push({0, startLabel});
    //while pq not empty
    while(!pq.empty())
    {
        //get weight and curvertex
        std::string curVertex = pq.top().second;
        pq.pop();
        //if visited then continue
        if(vertices[curVertex]->isVisited()) continue;
        //visit current vertex
        vertices[curVertex]->visit();
        //reset neighbor iterator
        vertices[curVertex]->resetNeighbor();
        //get v, the neighbor vertex
        std::string neighbor = vertices[curVertex]->getNextNeighbor();
        //while not to end of neighor list
        while(neighbor != vertices[curVertex]->getLabel())
        {
            //get the weight to v from curVertex
            int edgeWeight = vertices[curVertex]->getEdgeWeight(neighbor);
            int newCost = weight[curVertex] + edgeWeight;
            //if the new weight is lesss than old weight
            if(newCost < weight[neighbor])
            {
                //set new weight for vertex
                weight[neighbor] = newCost;
                //set the previous vertex of v to curVertex
                previous[neighbor] = curVertex;
                //push the new weight in
                pq.push({newCost, neighbor});
            }
            //set v to next neighbor in list
            neighbor = vertices[curVertex]->getNextNeighbor();
        }
        vertices[curVertex]->resetNeighbor();
    }
}

/** mark all verticies as unvisited */
void Graph::unvisitVertices()
{
    for(auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        if(it->second != nullptr) it->second->unvisit();
    }
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const
{
    //try to find vertex label, if it exists derefernce it and return
    //else return nullptr
    auto it = vertices.find(vertexLabel);
    if(it != vertices.end()) return it->second;
    return nullptr;
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel)
{
    //find vertex
    Vertex* v = findVertex(vertexLabel);
    //if it exists return it
    if(v != nullptr) return v;
    //if not then return new vertex with label
    numberOfVertices++;
    v = new Vertex(vertexLabel);
    vertices.insert({vertexLabel, v});
    return v;
}

#include <iostream>
#include <climits>
#include <map>
#include <sstream>
#include <vector>

#include "graph.h"

using namespace std;

// global variable - need better method, but works for testing
// used by graphVisitor
ostringstream graphOut;

// global variable for Djikstra
map<string, int> weight;
map<string, string> previous;

// OK or ERR for tests
template <typename T>
string isOK(const T& got, const T& expected) {
    if (got == expected) {
        return "OK: ";
    } else {
        cout << "    Got   " << got << "\n expected " << expected << endl;
        return "ERR: ";
    }
}

// visitor function - print out the vertex label
void display(const string& anItem) {
    cout << anItem << " ";
}

// visitor function - add the string to global variable graphOut
void graphVisitor(const string& anItem) {
    graphOut << anItem << " ";
}

// add the path to get to this vertex to global variable global variable
// [A B]
// previous is a map of vertexLabel prevVertexLabel
// need to process it in reverse to get the path
void graphCostDisplayPath(string vertex) {
    vector<string> v;
    auto prevIt = previous.find(vertex);
    while (prevIt != previous.end()) {
        v.push_back(prevIt->second);
        prevIt = previous.find(prevIt->second);
    }
    // v now has the path
    // oh, avoiding the joy of subtracting from unsigned int!
    int size = static_cast<int>(v.size());
    // return if we did we not go through any other vertex
    if (size <= 1) {
        return;
    }
    // add space between entries
    graphOut << "via [";
    // all entries except the first and last one in reverse
    // v[size-1] is the starting vertex, skipping it
    for (int i {size - 2}; i > 0; --i)
        graphOut << v[i] << " ";
    // add the second to last entry without space after it
    // last entry is the destination vertex, not needed
    if (size > 0)
        graphOut << v[0];
    graphOut << "] ";
}

// unreachable nodes should not even be in weight
// but if they are,
// skip them as they should have weight of INT_MAX
// C(8) via [A B]
// getting to C has a cost of 8, we can get to C via A->B->C
void graphCostDisplay() {
    graphOut.str("");
    for (auto it: weight) {
        int cost = it.second;
        if (cost == INT_MAX || cost == 0)
            continue;
        string vertex = it.first;
        graphOut << vertex << "(" << cost << ") ";
        graphCostDisplayPath(vertex);
    }
}

void testGraph0() {
    cout << "testGraph0" << endl;
    Graph g;
    g.readFile("graph0.txt");
    cout << isOK(g.getNumVertices(), 3) << "3 vertices" << endl;
    cout << isOK(g.getNumEdges(), 3) << "3 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C "s) << "DFS" << endl;
    
    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C "s) << "BFS" << endl;

    g.djikstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(), "B(1) C(4) via [B] "s)
         << "Djisktra" << endl << endl;
    
}

void testGraph1() {
    cout << "testGraph1" << endl;
    Graph g;
    g.readFile("graph1.txt");
    cout << isOK(g.getNumVertices(), 10) << "10 vertices" << endl;
    cout << isOK(g.getNumEdges(), 9) << "9 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C D E F G H "s) << "DFS" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B H C G D E F "s) << "BFS" << endl;

    g.djikstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "B(1) C(2) via [B] "s +
                 "D(3) via [B C] E(4) via [B C D] "s +
                 "F(5) via [B C D E] " +
                 "G(4) via [H] "s +
                 "H(3) "s)
         << "Djisktra" << endl << endl;
}

void testGraph2() {
    cout << "testGraph2" << endl;
    Graph g;

    g.readFile("graph2.txt");
    cout << isOK(g.getNumVertices(), 21) << "21 vertices" << endl;
    cout << isOK(g.getNumEdges(), 24) << "24 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B E F J C G K L D H M I N "s)
         << "DFS from A" << endl;

    graphOut.str("");
    g.depthFirstTraversal("O", graphVisitor);
    cout << isOK(graphOut.str(), "O P R S T U Q "s)
         << "DFS from O" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C D E F G H I J K L M N "s)
         << "BFS from A" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("D", graphVisitor);
    cout << isOK(graphOut.str(), "D H I M N "s)
         << "BFS from D" << endl;

    graphOut.str("");
    g.depthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), "U "s)
         << "DFS from U" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), "U "s)
         << "BFS from U" << endl;

    g.djikstraCostToAllVertices("O", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "P(5) Q(2) R(3) via [Q] S(6) via [Q R] " +
                 "T(8) via [Q R S] U(9) via [Q R S] "s)
         << "Djisktra" << endl << endl;
}

void testGraphEdgeCases() {
    cout << "testGraphEdgeCases" << endl;
    
    //bad start label
    
    Graph g1;
    g1.readFile("graph0.txt");
    
    graphOut.str("");
    g1.depthFirstTraversal("X", graphVisitor);
    cout << isOK(graphOut.str(), ""s) << "Bad start DFS" << endl;
    
    graphOut.str("");
    g1.breadthFirstTraversal("X", graphVisitor);
    cout << isOK(graphOut.str(), ""s) << "Bad start BFS" << endl;
    
    weight.clear();
    previous.clear();
    g1.djikstraCostToAllVertices("X", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(), ""s) << "Bad start Dijkstra" << endl;
    

    //disconnected graph
    
    Graph g2;
    g2.add("A", "B", 1);
    g2.add("C", "D", 1);  //disconnected
    
    graphOut.str("");
    g2.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B "s) << "Disconnected DFS" << endl;
    
    graphOut.str("");
    g2.breadthFirstTraversal("C", graphVisitor);
    cout << isOK(graphOut.str(), "C D "s) << "Disconnected BFS" << endl;
    
    weight.clear();
    previous.clear();
    g2.djikstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(), "B(1) "s) << "Disconnected Dijkstra" << endl;


    //duplicate edges and self edges
    
    Graph g3;
    cout << isOK(g3.add("A", "A", 5), false) << "Self edge prevention" << endl;
    cout << isOK(g3.add("A", "B"), true) << "First add" << endl;
    cout << isOK(g3.add("A", "B"), false) << "Duplicate add" << endl;
    cout << isOK(g3.getNumEdges(), 1) << "Edge count after duplicate add" << endl;
    
    
    cout << endl;
}

int main() {
    testGraph0();
    testGraph1();
    testGraph2();
    testGraphEdgeCases();  // Add new test suite
    return 0;
}

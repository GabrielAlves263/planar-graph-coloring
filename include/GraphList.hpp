#ifndef GraphList_HPP
#define GraphList_HPP

#include <list>
#include <vector>
#include <unordered_map>

using namespace std;

typedef int TypeItem;

class GraphList
{
private:
    int V;
    bool directedGraph;
    vector<TypeItem> vertices;
    vector<list<TypeItem>> adj;
    vector<int> colors;

public:
    GraphList(int V, bool directedGraph = false); // Constructor
    ~GraphList();                                 // Destructor
    vector<TypeItem> getVertices();
    vector<list<TypeItem>> getAdj();
    void insertVertex(TypeItem v);
    int getIndex(TypeItem v);
    int getDegree(TypeItem v);
    void insertEdge(TypeItem v1, TypeItem v2);
    void printGraph();
    bool isFull();

    // Coloração por força bruta
    bool isColoringValid(vector<int> colors);
    int bruteForceColoring();

    // Coloração com heurística de Grundy
    vector<int> grundyColoring();
};

#endif
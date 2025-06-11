#ifndef GraphList_HPP
#define GraphList_HPP

#include <list>
#include <vector>

using namespace std;

// Tipo de dado que o grafo irá guardar
typedef int TypeItem;

class GraphList
{
private:
    int V;
    bool directedGraph;
    vector<TypeItem> vertex;
    vector<list<TypeItem>> adj;
    vector<int> color;

public:
    GraphList(int V, bool directedGraph = false); // Constructor
    ~GraphList();                                 // Destructor
    void insertVertex(TypeItem v);
    int getIndex(TypeItem v);
    int getDegree(TypeItem v);
    void insertEdge(TypeItem v1, TypeItem v2);
    void printGraph();
    bool isFull();
};

#endif
#ifndef GraphList_HPP
#define GraphList_HPP

#include <list>
#include <vector>

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
    void insertVertex(TypeItem v);
    int getIndex(TypeItem v);
    int getDegree(TypeItem v);
    void insertEdge(TypeItem v1, TypeItem v2);
    void printGraph();
    bool isFull();
};

#endif
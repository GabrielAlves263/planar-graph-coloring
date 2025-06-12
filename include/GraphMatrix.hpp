#ifndef GraphMatrix_HPP
#define GraphMatrix_HPP

#include <vector>

using namespace std;

typedef int TypeItem;

class GraphMatrix
{
private:
    int V;
    vector<TypeItem> vertices;
    vector<vector<int>> adjMatrix;
    vector<int> colors;

public:
    GraphMatrix(int V);
    ~GraphMatrix();
    int getIndex(TypeItem v);
    bool isFull();
    void insertVertex(TypeItem v);
    void insertEdge(TypeItem v1, TypeItem v2);
    int getDegree(TypeItem v1);
    void printGraph();
};

#endif
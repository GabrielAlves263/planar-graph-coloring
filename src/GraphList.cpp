#include "GraphList.hpp"
#include <iostream>

using namespace std;

GraphList::GraphList(int V, bool directedGraph)
{
    this->V = V;
    this->directedGraph = directedGraph;
    adj.resize(V);
}

GraphList::~GraphList()
{
    vertices.clear();
    adj.clear();
}

void GraphList::insertVertex(TypeItem v)
{
    if (isFull())
    {
        cout << "Graph is full!" << endl;
        return;
    }

    vertices.push_back(v);
    adj.resize(vertices.size());
}

int GraphList::getIndex(TypeItem v)
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] == v)
        {
            return i;
        }
    }

    // If vertices was not found
    return -1;
}

void GraphList::insertEdge(TypeItem v1, TypeItem v2)
{
    int index1 = getIndex(v1);
    int index2 = getIndex(v2);

    if (index1 == -1 || index2 == -1)
    {
        cout << "Vertex not found" << endl;
        return;
    }

    adj[index1].push_back(v2);

    if (!directedGraph)
        adj[index2].push_back(v1);
}

int GraphList::getDegree(TypeItem v)
{
    int index = getIndex(v);
    if (index != -1)
    {
        return adj[index].size();
    }
    return -1;
}

void GraphList::printGraph()
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        cout << "Vertex " << i << ": ";
        for (TypeItem neighbor : adj[i])
        {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

bool GraphList::isFull()
{
    return vertices.size() == static_cast<size_t>(V);
}
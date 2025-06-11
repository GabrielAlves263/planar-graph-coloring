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
    vertex.clear();
    adj.clear();
}

void GraphList::insertVertex(TypeItem v)
{
    if (isFull())
    {
        cout << "O gráfico está cheio!" << endl;
        return;
    }

    vertex.push_back(v);
    adj.resize(vertex.size());
}

int GraphList::getIndex(TypeItem v)
{
    for (size_t i = 0; i < vertex.size(); i++)
    {
        if (vertex[i] == v)
        {
            return i;
        }
    }

    // If vertex was not found
    return -1;
}

void GraphList::insertEdge(TypeItem v1, TypeItem v2)
{
    int index1 = getIndex(v1);
    int index2 = getIndex(v2);

    if (index1 == -1 || index2 == -1)
    {
        cout << "Vertex not found";
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
    for (size_t i = 0; i < vertex.size(); i++)
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
    return vertex.size() == static_cast<size_t>(V);
}
#include "GraphList.hpp"
#include <iostream>

using namespace std;

GraphList::GraphList(int V)
{
    this->V = V;
    adj = new list<int>[this->V];
}

void GraphList::adcionarAresta(int v1, int v2)
{
    adj[v1].push_back(v2);
    adj[v2].push_back(v1);
}

int GraphList::obterGrau(int v)
{
    return adj[v].size();
}

void GraphList::imprimir()
{
    for (int v = 0; v < V; v++)
    {
        cout << "Vertice " << v << ": ";
        for (int vizinho : adj[v])
        {
            cout << vizinho << " ";
        }
        cout << endl;
    }
}
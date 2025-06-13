#include "GraphList.hpp"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <set>

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

vector<TypeItem> GraphList::getVertices()
{
    return this->vertices;
}

vector<list<TypeItem>> GraphList::getAdj()
{
    return this->adj;
}

bool GraphList::isColoringValid(vector<int> colors)
{
    for (size_t u = 0; u < vertices.size(); u++)
    {
        for (auto &v : adj[u])
        {
            int idx = getIndex(v);
            if (idx != -1 && colors[u] == colors[idx])
                return false;
        }
    }
    return true;
}

int GraphList::bruteForceColoring()
{
    size_t n = vertices.size();

    for (int k = 1; k <= n; k++)
    {
        vector<int> colors(n, 0);

        while (true)
        {
            if (isColoringValid(colors))
                return k;

            int i = n - 1;
            while (i >= 0)
            {
                if (colors[i] < k - 1)
                {
                    colors[i]++;
                    break;
                }
                else
                {
                    colors[i] = 0;
                    i--;
                }
            }

            if (i < 0)
                break;
        }
    }

    return n;
}

vector<int> GraphList::grundyColoring()
{
    int n = vertices.size();
    colors.assign(n, -1);

    vector<int> ordem(n);
    for (int i = 0; i < n; i++)
        ordem[i] = i;

    sort(ordem.begin(), ordem.end(), [&](int a, int b)
         { return adj[a].size() > adj[b].size(); });

    for (int idx : ordem)
    {
        set<int> cores_usadas;
        for (auto vizinho : adj[idx])
        {
            int vizIdx = getIndex(vizinho);
            if (vizIdx != -1 && colors[vizIdx] != -1)
                cores_usadas.insert(colors[vizIdx]);
        }

        int cor = 0;
        while (cores_usadas.find(cor) != cores_usadas.end())
            cor++;

        colors[idx] = cor;
    }
    return colors;
}
#include "GraphMatrix.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

GraphMatrix::GraphMatrix(int V)
{
    this->V = V;
    adjMatrix = vector<vector<int>>(V, vector<int>(V, 0));
}

GraphMatrix::~GraphMatrix()
{
    vertices.clear();
    adjMatrix.clear();
}

int GraphMatrix::getIndex(TypeItem v)
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

bool GraphMatrix::isFull()
{
    return vertices.size() == static_cast<size_t>(V);
}

void GraphMatrix::insertVertex(TypeItem v)
{
    if (isFull())
    {
        cout << "Graph is full!" << endl;
        return;
    }

    vertices.push_back(v);

    size_t newSize = vertices.size();
    adjMatrix.resize(newSize);
    for (auto &row : adjMatrix)
    {
        row.resize(newSize, 0);
    }
}

void GraphMatrix::insertEdge(TypeItem v1, TypeItem v2)
{
    int line = getIndex(v1);
    int column = getIndex(v2);

    if (line == -1 || column == -1)
    {
        cout << "Vertex not found" << endl;
        return;
    }
    else if (line == column)
    {
        cout << "Cannot insert edge to itself" << endl;
        return;
    }
    else if (adjMatrix[line][column] != 0)
    {
        cout << "Edge already exists" << endl;
        return;
    }

    adjMatrix[line][column] = 1;
    adjMatrix[column][line] = 1;
}

int GraphMatrix::getDegree(TypeItem v)
{
    int index = getIndex(v);
    if (index == -1)
    {
        cout << "Vertex not found" << endl;
        return -1;
    }
    else
    {
        int degree = 0;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            if (adjMatrix[index][i] != 0)
                degree++;
        }
        return degree;
    }
}

void GraphMatrix::printGraph()
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices.size(); j++)
        {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

vector<TypeItem> GraphMatrix::getVertices()
{
    return this->vertices;
}

vector<vector<int>> GraphMatrix::getAdjMatrix()
{
    return this->adjMatrix;
}

vector<int> GraphMatrix::grundyColoring()
{
    int n = vertices.size();
    vector<int> colors(n, -1);

    vector<int> ordem(n);
    for (int i = 0; i < n; i++)
    {
        ordem[i] = i;
    }

    sort(ordem.begin(), ordem.end(), [&](int a, int b)
         {
        int grauA = getDegree(a);
        int grauB = getDegree(b);
    
    return grauA > grauB; });

    for (int idx : ordem)
    {
        set<int> used;

        for (int j = 0; j < n; j++)
        {
            if (adjMatrix[idx][j] && colors[j] != -1)
                used.insert(colors[j]);
        }

        int color = 0;
        while (used.find(color) != used.end())
            color++;

        colors[idx] = color;
    }

    return colors;
}

int isSafe(const vector<vector<int>> &adjMatrix, const vector<int> &colors, int vertex, int color)
{
    for (size_t i = 0; i < adjMatrix.size(); i++)
    {
        if (adjMatrix[vertex][i] && colors[i] == color)
            return false;
    }
    return true;
}

bool bruteForceUtil(const vector<vector<int>> &adjMatrix, vector<int> &colors, int vertex, int maxColors)
{
    if (vertex == adjMatrix.size())
        return true;

    for (int c = 0; c < maxColors; c++)
    {
        if (isSafe(adjMatrix, colors, vertex, c))
        {
            colors[vertex] = c;
            if (bruteForceUtil(adjMatrix, colors, vertex + 1, maxColors))
                return true;
            colors[vertex] = -1;
        }
    }
    return false;
}

int GraphMatrix::bruteForceColoring()
{
    int n = vertices.size();
    vector<int> colors(n, -1);

    for (int maxColors = 1; maxColors <= n; maxColors++)
    {
        fill(colors.begin(), colors.end(), -1);
        if (bruteForceUtil(adjMatrix, colors, 0, maxColors))
        {
            return maxColors;
        }
    }
    return n;
}

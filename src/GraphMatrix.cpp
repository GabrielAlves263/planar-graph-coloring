#include "GraphMatrix.hpp"
#include <iostream>
#include <vector>

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
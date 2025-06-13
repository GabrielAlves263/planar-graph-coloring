#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>

using namespace std;

auto generatePlanarGraph(int n) -> pair<set<int>, vector<pair<int, int>>>
{
    int k = sqrt(n);

    set<int> vertices;
    vector<pair<int, int>> edges;
    int id = 0;

    auto degree = [&](int v)
    {
        int count = 0;
        for (const auto &a : edges)
        {
            if (a.first == v || a.second == v)
                count++;
        }
        return count;
    };

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            vertices.insert(id);
            if (i < k - 1)
                edges.push_back({id, id + k});
            if (j < k - 1)
                edges.push_back({id, id + 1});
            id++;
        }
    }

    if (k * k > n)
    {
        for (int r = n + 1; r <= k * k; r++)
        {
            vertices.erase(r);

            edges.erase(
                remove_if(edges.begin(), edges.end(), [r](const pair<int, int> &edge)
                          { return edge.first == r || edge.second == r; }),
                edges.end());
        }
    }
    else if (k * k < n)
    {
        for (int r = k * k + 1; r <= n; r++)
        {
            vertices.insert(r);

            int foundVertex = -1;
            for (int v : vertices)
            {
                if (v == r)
                    continue;
                if (degree(v) < 4)
                {
                    foundVertex = v;
                    break;
                }
            }

            if (foundVertex != -1)
                edges.push_back({r, foundVertex});
        }
    }

    return {vertices, edges};
}

bool isValidColoring(const vector<int> &color, const vector<int> &vertices, const vector<list<int>> &adj)
{
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (const auto &neighbor : adj[i])
        {
            int j = find(vertices.begin(), vertices.end(), neighbor) - vertices.begin();
            if (color[i] == color[j])
                return false;
        }
    }
    return true;
}

bool bruteForceColoring(const vector<int> &vertices, const vector<list<int>> &adj, int k, vector<int> &color, size_t pos = 0)
{
    if (pos == vertices.size())
    {
        return isValidColoring(color, vertices, adj);
    }

    for (int c = 1; c <= k; c++)
    {
        color[pos] = c;
        if (bruteForceColoring(vertices, adj, k, color, pos + 1))
            return true;
    }
    return false;
}

int findChromaticNumber(const vector<int> &vertices, const vector<list<int>> &adj)
{
    for (size_t k = 1; k <= vertices.size(); k++)
    {
        vector<int> color(vertices.size(), 0);
        if (bruteForceColoring(vertices, adj, k, color))
        {
            return k;
        }
    }

    return -1;
}

int main()
{
    int n = 30;
    auto planarGraph = generatePlanarGraph(n);
    auto vertices = planarGraph.first;
    auto edges = planarGraph.second;

    GraphList graphList(n);
    GraphMatrix graphMatrix(n);

    for (auto &v : vertices)
    {
        graphList.insertVertex(v);
        graphMatrix.insertVertex(v);
    }

    for (auto &a : edges)
    {
        graphList.insertEdge(a.first, a.second);
        graphMatrix.insertEdge(a.first, a.second);
    }

    graphList.printGraph();

    int chromaticNumber = findChromaticNumber(graphList.getVertices(), graphList.getAdj());

    cout << "Chromatic Number: " << chromaticNumber << endl;

    return 0;
}
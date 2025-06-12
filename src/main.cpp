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

int main()
{
    int n = 500;
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

    graphMatrix.printGraph();

    return 0;
}
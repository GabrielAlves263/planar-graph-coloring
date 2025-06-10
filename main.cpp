#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include "GrafoLista.hpp"

using namespace std;

auto gerarGrafoPlanar(int n) -> pair<set<int>, vector<pair<int, int>>>
{
    int k = sqrt(n);

    set<int> vertices;
    vector<pair<int, int>> arestas;
    int id = 0;

    auto grau = [&](int v)
    {
        int count = 0;
        for (const auto &a : arestas)
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
                arestas.push_back({id, id + k});
            if (j < k - 1)
                arestas.push_back({id, id + 1});
            id++;
        }
    }

    if (k * k > n)
    {
        for (int r = n + 1; r <= k * k; r++)
        {
            vertices.erase(r);

            arestas.erase(
                remove_if(arestas.begin(), arestas.end(), [r](const pair<int, int> &aresta)
                          { return aresta.first == r || aresta.second == r; }),
                arestas.end());
        }
    }
    else if (k * k < n)
    {
        for (int r = k * k + 1; r <= n; r++)
        {
            vertices.insert(r);

            int v_achado = -1;
            for (int v : vertices)
            {
                if (v == r)
                    continue;
                if (grau(v) < 4)
                {
                    v_achado = v;
                    break;
                }
            }

            if (v_achado != -1)
                arestas.push_back({r, v_achado});
        }
    }

    return {vertices, arestas};
}

int main()
{
    int n = 500;
    auto grafoPlanar = gerarGrafoPlanar(n);
    auto vertices = grafoPlanar.first;
    auto arestas = grafoPlanar.second;

    int maxV = *max_element(vertices.begin(), vertices.end());
    Grafo grafo(maxV + 1);

    for (auto &v : arestas)
    {
        grafo.adcionarAresta(v.first, v.second);
    }

    grafo.imprimir();

    return 0;
}
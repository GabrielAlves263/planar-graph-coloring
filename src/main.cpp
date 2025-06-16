#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>
#include "GraphList.hpp"
#include "GraphMatrix.hpp"

using namespace std;
using namespace std::chrono;

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

int main() {
    // Abre o arquivo CSV para escrita e adiciona o cabeçalho
    ofstream csvFile("resultados_benchmark.csv");
    csvFile << "N,Algoritmo,Representacao,Tempo_Medio_ns\n";

    // =================================================================
    // PARTE 1: Algoritmo de Grundy
    // =================================================================
    const int REPETICOES_GRUNDY = 1000000;
    vector<int> n_values_grundy = {500, 1000, 2000}; // Valores originais do seu experimento

    for (int n : n_values_grundy) {
        cout << "Processando n = " << n << " para o algoritmo de Grundy..." << endl;

        auto graphData = generatePlanarGraph(n);
        auto vertices = graphData.first;
        auto edges = graphData.second;

        GraphList graphList(n);
        for (int v : vertices) graphList.insertVertex(v);
        for (const auto& edge : edges) graphList.insertEdge(edge.first, edge.second);

        GraphMatrix graphMatrix(n);
        for (int v : vertices) graphMatrix.insertVertex(v);
        for (const auto& edge : edges) graphMatrix.insertEdge(edge.first, edge.second);

        // Medir tempo para Grundy + Lista
        long long total_time_grundy_list = 0;
        for (int i = 0; i < REPETICOES_GRUNDY; ++i) {
            auto start = high_resolution_clock::now();
            graphList.grundyColoring();
            auto stop = high_resolution_clock::now();
            total_time_grundy_list += duration_cast<nanoseconds>(stop - start).count();
        }
        double avg_time_grundy_list = static_cast<double>(total_time_grundy_list) / REPETICOES_GRUNDY;
        csvFile << n << ",Grundy,Lista," << avg_time_grundy_list << "\n";
        cout << "  Grundy + Lista: " << avg_time_grundy_list << " ns" << endl;


        // Medir tempo para Grundy + Matriz
        long long total_time_grundy_matrix = 0;
        for (int i = 0; i < REPETICOES_GRUNDY; ++i) {
            auto start = high_resolution_clock::now();
            graphMatrix.grundyColoring();
            auto stop = high_resolution_clock::now();
            total_time_grundy_matrix += duration_cast<nanoseconds>(stop - start).count();
        }
        double avg_time_grundy_matrix = static_cast<double>(total_time_grundy_matrix) / REPETICOES_GRUNDY;
        csvFile << n << ",Grundy,Matriz," << avg_time_grundy_matrix << "\n";
        cout << "  Grundy + Matriz: " << avg_time_grundy_matrix << " ns" << endl;
    }

    // =================================================================
    // PARTE 2: Algoritmo de Força Bruta com múltiplos 'n'
    // =================================================================
    
    // ATENÇÃO: Repetições reduzidas para o Força Bruta para evitar tempo de execução excessivo.
    const int REPETICOES_BRUTE = 10000; 
    vector<int> n_values_brute = {8, 10, 12, 14}; // Lista de 'n' para o teste

    // Laço para iterar sobre cada valor de 'n' do Força Bruta
    for (int n_brute : n_values_brute) {
        cout << "\nProcessando n = " << n_brute << " para o algoritmo de Forca Bruta..." << endl;

        // 1. Gerar o grafo UMA VEZ para o 'n' atual
        auto graphDataBrute = generatePlanarGraph(n_brute);
        auto verticesBrute = graphDataBrute.first;
        auto edgesBrute = graphDataBrute.second;

        GraphList graphListBrute(n_brute);
        for (int v : verticesBrute) graphListBrute.insertVertex(v);
        for (const auto& edge : edgesBrute) graphListBrute.insertEdge(edge.first, edge.second);

        GraphMatrix graphMatrixBrute(n_brute);
        for (int v : verticesBrute) graphMatrixBrute.insertVertex(v);
        for (const auto& edge : edgesBrute) graphMatrixBrute.insertEdge(edge.first, edge.second);

        // 2. Medir tempo para Força Bruta + Lista
        long long total_time_brute_list = 0;
        for (int i = 0; i < REPETICOES_BRUTE; ++i) {
            auto start = high_resolution_clock::now();
            graphListBrute.bruteForceColoring();
            auto stop = high_resolution_clock::now();
            total_time_brute_list += duration_cast<nanoseconds>(stop - start).count();
        }
        double avg_time_brute_list = static_cast<double>(total_time_brute_list) / REPETICOES_BRUTE;
        csvFile << n_brute << ",Forca Bruta,Lista," << avg_time_brute_list << "\n";
        cout << "  Forca Bruta + Lista: " << avg_time_brute_list << " ns" << endl;

        // 3. Medir tempo para Força Bruta + Matriz
        long long total_time_brute_matrix = 0;
        for (int i = 0; i < REPETICOES_BRUTE; ++i) {
            auto start = high_resolution_clock::now();
            graphMatrixBrute.bruteForceColoring();
            auto stop = high_resolution_clock::now();
            total_time_brute_matrix += duration_cast<nanoseconds>(stop - start).count();
        }
        double avg_time_brute_matrix = static_cast<double>(total_time_brute_matrix) / REPETICOES_BRUTE;
        csvFile << n_brute << ",Forca Bruta,Matriz," << avg_time_brute_matrix << "\n";
        cout << "  Forca Bruta + Matriz: " << avg_time_brute_matrix << " ns" << endl;
    }

    csvFile.close();
    cout << "\nBenchmark concluido. Resultados salvos em 'resultados_benchmark.csv'" << endl;

    return 0;
}
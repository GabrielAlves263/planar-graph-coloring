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
#include <omp.h>

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

// Uma estrutura para guardar os resultados de cada teste
struct BenchmarkResult {
    int n;
    string algorithm;
    string representation;
    double avg_time_ns;
};

int main() {
    vector<BenchmarkResult> all_results;
    
    const int REPETICOES_GRUNDY = 1000000;
    vector<int> n_values_grundy = {500, 1000, 2000};
    
    cout << "Starting Grundy benchmarks (in parallel)..." << endl;

    #pragma omp parallel for
    for (int i = 0; i < n_values_grundy.size(); ++i) {
        int n = n_values_grundy[i];
        
        #pragma omp critical
        {
            cout << "  Thread " << omp_get_thread_num() << " processing n = " << n << " for Grundy..." << endl;
        }

        auto graphData = generatePlanarGraph(n);
        auto vertices = graphData.first;
        auto edges = graphData.second;

        GraphList graphList(n);
        for (int v : vertices) graphList.insertVertex(v);
        for (const auto& edge : edges) graphList.insertEdge(edge.first, edge.second);
        
        long long total_time_list = 0;
        for (int rep = 0; rep < REPETICOES_GRUNDY; ++rep) {
            auto start = high_resolution_clock::now();
            graphList.grundyColoring();
            auto stop = high_resolution_clock::now();
            total_time_list += duration_cast<nanoseconds>(stop - start).count();
        }
        
        GraphMatrix graphMatrix(n);
        for (int v : vertices) graphMatrix.insertVertex(v);
        for (const auto& edge : edges) graphMatrix.insertEdge(edge.first, edge.second);
        
        long long total_time_matrix = 0;
        for (int rep = 0; rep < REPETICOES_GRUNDY; ++rep) {
            auto start = high_resolution_clock::now();
            graphMatrix.grundyColoring();
            auto stop = high_resolution_clock::now();
            total_time_matrix += duration_cast<nanoseconds>(stop - start).count();
        }

        #pragma omp critical
        {
            cout << "    [DONE] Grundy + List, n = " << n << endl;
            all_results.push_back({n, "Grundy", "List", static_cast<double>(total_time_list) / REPETICOES_GRUNDY});
            
            cout << "    [DONE] Grundy + Matrix, n = " << n << endl;
            all_results.push_back({n, "Grundy", "Matrix", static_cast<double>(total_time_matrix) / REPETICOES_GRUNDY});
        }
    }

    const int REPETICOES_BRUTE = 10000;
    vector<int> n_values_brute = {8, 10, 12, 14};
    
    cout << "\nStarting Brute Force benchmarks (in parallel)..." << endl;

    #pragma omp parallel for
    for (int i = 0; i < n_values_brute.size(); ++i) {
        int n_brute = n_values_brute[i];
        
        #pragma omp critical
        {
            cout << "  Thread " << omp_get_thread_num() << " processing n = " << n_brute << " for Brute Force..." << endl;
        }

        auto graphDataBrute = generatePlanarGraph(n_brute);
        auto verticesBrute = graphDataBrute.first;
        auto edgesBrute = graphDataBrute.second;

        GraphList graphListBrute(n_brute);
        for (int v : verticesBrute) graphListBrute.insertVertex(v);
        for (const auto& edge : edgesBrute) graphListBrute.insertEdge(edge.first, edge.second);

        long long total_time_brute_list = 0;
        for (int rep = 0; rep < REPETICOES_BRUTE; ++rep) {
            auto start = high_resolution_clock::now();
            graphListBrute.bruteForceColoring();
            auto stop = high_resolution_clock::now();
            total_time_brute_list += duration_cast<nanoseconds>(stop - start).count();
        }

        GraphMatrix graphMatrixBrute(n_brute);
        for (int v : verticesBrute) graphMatrixBrute.insertVertex(v);
        for (const auto& edge : edgesBrute) graphMatrixBrute.insertEdge(edge.first, edge.second);

        long long total_time_brute_matrix = 0;
        for (int rep = 0; rep < REPETICOES_BRUTE; ++rep) {
            auto start = high_resolution_clock::now();
            graphMatrixBrute.bruteForceColoring();
            auto stop = high_resolution_clock::now();
            total_time_brute_matrix += duration_cast<nanoseconds>(stop - start).count();
        }

        #pragma omp critical
        {
            cout << "    [DONE] Brute Force + List, n = " << n_brute << endl;
            all_results.push_back({n_brute, "Brute Force", "List", static_cast<double>(total_time_brute_list) / REPETICOES_BRUTE});
            
            cout << "    [DONE] Brute Force + Matrix, n = " << n_brute << endl;
            all_results.push_back({n_brute, "Brute Force", "Matrix", static_cast<double>(total_time_brute_matrix) / REPETICOES_BRUTE});
        }
    }

    cout << "\nAll benchmarks concluded. Saving results..." << endl;
    
    sort(all_results.begin(), all_results.end(), [](const BenchmarkResult& a, const BenchmarkResult& b) {
        if (a.algorithm != b.algorithm) return a.algorithm > b.algorithm;
        if (a.n != b.n) return a.n < b.n;
        return a.representation < b.representation;
    });

    ofstream csvFile("benchmark.csv");
    csvFile << "N,Algorithm,Representation,Avg_Time_ns\n";
    for (const auto& res : all_results) {
        csvFile << res.n << "," << res.algorithm << "," << res.representation << "," << res.avg_time_ns << "\n";
    }
    csvFile.close();

    cout << "\nBenchmark finished. Results saved to 'benchmark.csv'" << endl;

    return 0;
}
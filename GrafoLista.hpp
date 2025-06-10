#ifndef GRAFOLISTA_HPP
#define GRAFOLISTA_HPP

#include <list>
using namespace std;

class Grafo
{
    int V;
    list<int> *adj;

public:
    Grafo(int V);
    void adcionarAresta(int v1, int v2);
    int obterGrau(int v);
    void imprimir();
};

#endif
#ifndef GraphList_HPP
#define GraphList_HPP

#include <list>
using namespace std;

class GraphList
{
    int V;
    list<int> *adj;

public:
    GraphList(int V);
    void adcionarAresta(int v1, int v2);
    int obterGrau(int v);
    void imprimir();
};

#endif
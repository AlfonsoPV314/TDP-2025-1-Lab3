#ifndef COMPARADOR_NODOS_H
#define COMPARADOR_NODOS_H
#include <memory>
#include "NodoArbol.h"

class ComparadorNodos {
public:
    bool operator()(const shared_ptr<NodoArbol>& a, const shared_ptr<NodoArbol>& b);
};

#endif // COMPARADOR_NODOS_H
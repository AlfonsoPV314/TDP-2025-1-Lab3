#ifndef STRATEGY_RESOLUCION_H
#define STRATEGY_RESOLUCION_H

#include "Problema.h"
#include "NodoArbol.h"
#include <memory>
#include <vector>

using namespace std;

class ResultadoSolucion {
public:
    double valorObjetivo;
    vector<double> solucionFinal;
    bool esFactible;
    int numIteraciones;
    double tiempoEjecucion;
    
    ResultadoSolucion() : valorObjetivo(1e9), esFactible(false), numIteraciones(0), tiempoEjecucion(0.0) {}
};

class StrategyResolucion {
public:
    virtual ~StrategyResolucion() = default;
    virtual ResultadoSolucion resolver(const Problema& problema) = 0;
    virtual string getNombre() const = 0;
};

#endif // STRATEGY_RESOLUCION_H
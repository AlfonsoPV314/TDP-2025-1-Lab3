#ifndef STRATEGY_BRANCH_AND_BOUND_H
#define STRATEGY_BRANCH_AND_BOUND_H

#include "StrategyResolucion.h"
#include "ComparadorNodos.h"
#include <coin/ClpSimplex.hpp>          
#include <coin/CoinPackedMatrix.hpp>
#include <coin/CoinPackedVector.hpp> 
#include <queue>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <cfloat>

using namespace std;

class StrategyBranchAndBound : public StrategyResolucion {
private:
    int maxIteraciones;
    double tolerancia;
    double demandaTotal;
    
    priority_queue<shared_ptr<NodoArbol>, vector<shared_ptr<NodoArbol>>, ComparadorNodos> colaNodos;
    
    void resolverRelajacionLP(const Problema& problema, shared_ptr<NodoArbol> nodo);
    double calcularCostoExacto(const Problema& problema, const vector<double>& solucion);
    vector<double> getSolucionGreedy(const Problema& problema);
    void podarNodos(double mejorCotaSup);
    bool esOptimo(double cotaInf, double cotaSup) const;
    
public:
    StrategyBranchAndBound(const Problema& problema, int maxIter, double tol);
    
    ResultadoSolucion resolver(const Problema& problema) override;
    string getNombre() const override { return "Ramificación y Acotamiento Mejorado"; }
};

#endif // STRATEGY_BRANCH_AND_BOUND_H
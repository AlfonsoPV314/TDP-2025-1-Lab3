#ifndef RESUELVE_CFL_H
#define RESUELVE_CFL_H

#include "StrategyResolucion.h"
#include <memory>
#include <stdexcept>

using namespace std;

class ResolveCFL {
private:
    shared_ptr<StrategyResolucion> Strategy;

public:
    ResolveCFL(shared_ptr<StrategyResolucion> Strategy);
    
    ResultadoSolucion resolver(const Problema& problema);
    string getNombreStrategy() const;
};

#endif // RESUELVE_CFL_H
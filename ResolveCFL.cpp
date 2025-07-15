#include "ResolveCFL.h"

/**
 * Desc: Constructor de la clase ResolveCFL.
 *       Inicializa la estrategia de resolución utilizada para resolver el problema CFL.
 * Params:
 *   - strategy: Puntero compartido a la estrategia de resolución (por ejemplo, Branch and Bound).
 * Returns: void
 */
ResolveCFL::ResolveCFL(shared_ptr<StrategyResolucion> Strategy) : Strategy(Strategy) {}

/**
 * Desc: Resuelve el problema utilizando la estrategia de resolución actual.
 *       Llama al método `resolver` de la estrategia asignada.
 * Params:
 *   - problema: Referencia constante al problema a resolver.
 * Returns:
 *   - ResultadoSolucion: Estructura con los resultados del problema resuelto,
 *                         incluyendo solución, costo y tiempo de ejecución.
 * Throws:
 *   - runtime_error: Si no se ha establecido ninguna estrategia de resolución.
 */
ResultadoSolucion ResolveCFL::resolver(const Problema& problema) {
    if (!Strategy) {
        throw runtime_error("No se ha establecido una estrategia de resolución");
    }
    
    return Strategy->resolver(problema);
}

/**
 * Desc: Obtiene el nombre de la estrategia de resolución actual.
 *       Si no hay estrategia asignada, devuelve "Sin estrategia".
 * Params: void
 * Returns:
 *   - string: Nombre de la estrategia actual.
 */
string ResolveCFL::getNombreStrategy() const {
    return Strategy ? Strategy->getNombre() : "Sin estrategia";
}
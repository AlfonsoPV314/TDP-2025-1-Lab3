#include "ComparadorNodos.h"

/**
 * Desc: Compara dos nodos del árbol según su cota inferior.
 *       Utilizado para ordenar nodos en una cola de prioridad.
 * Params:
 *   - a: Primer nodo a comparar.
 *   - b: Segundo nodo a comparar.
 * Returns:
 *   - bool: True si la cota inferior de 'a' es mayor que la de 'b',
 *           False en caso contrario.
 */
bool ComparadorNodos::operator()(const shared_ptr<NodoArbol>& a, const shared_ptr<NodoArbol>& b) {
    return a->obtenerCotaInferior() > b->obtenerCotaInferior();
}
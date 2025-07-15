#include "NodoArbol.h"
#include "ComparadorNodos.h"
#include <iostream>
#include <vector>
#include <queue>
#include <memory>

using namespace std;

/**
 * Desc: Test unitario para la clase ComparadorNodos.
 *       Verifica que los nodos se comparen correctamente según su cota inferior.
 * 
 * Funcionalidades verificadas:
 *   - El comparador ordena los nodos de menor a mayor cota inferior.
 *   - Funciona correctamente dentro de una priority_queue.
 */

int main() {
    cout << "=== Iniciando testComparadorNodos ===\n";

    // Crear un comparador
    ComparadorNodos comparador;

    // Crear nodos con cotas inferiores diferentes
    auto nodo1 = make_shared<NodoArbol>();
    auto nodo2 = make_shared<NodoArbol>();
    auto nodo3 = make_shared<NodoArbol>();

    nodo1->establecerCotaInferior(150.0);
    nodo2->establecerCotaInferior(100.0);
    nodo3->establecerCotaInferior(200.0);

    // Usar cola de prioridad con ComparadorNodos
    priority_queue<shared_ptr<NodoArbol>, vector<shared_ptr<NodoArbol>>, ComparadorNodos> cola(comparador);

    // Insertar nodos desordenados
    cola.push(nodo3);
    cola.push(nodo1);
    cola.push(nodo2);

    cout << "[testComparadorNodos] Cola priorizada - Orden esperado: 100.0, 150.0, 200.0\n";

    int i = 0;
    while (!cola.empty()) {
        auto nodo = cola.top();
        cout << "[testComparadorNodos] Nodo " << i++ << " - Cota inferior: " << nodo->obtenerCotaInferior() << "\n";
        cola.pop();
    }

    cout << "=== testComparadorNodos finalizado ===\n";
    return 0;
}
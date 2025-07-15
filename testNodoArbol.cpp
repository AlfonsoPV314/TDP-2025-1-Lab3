#include "NodoArbol.h"  
#include <iostream>    
#include <memory>      

int main() {
    // === 1. Crear nodo raíz y configurar propiedades básicas ===
    auto raiz = std::make_shared<NodoArbol>();
    raiz->establecerCotaInferior(100.5);
    raiz->establecerCotaSuperior(200.5);
    raiz->fijarVariable(0, 1);  // Fijar variable y₀ = 1
    raiz->establecerProfundidad(0);

    // Configurar solución continua {0.5, 1.0, 0.0}
    std::vector<double> sol = {0.5, 1.0, 0.0};
    raiz->establecerSolucion(sol);

    // === 2. Verificar getters básicos ===
    std::cout << "Cota inferior: " << raiz->obtenerCotaInferior() << "\n";
    std::cout << "Cota superior: " << raiz->obtenerCotaSuperior() << "\n";
    
    // === 3. Verificar estado del nodo ===
    std::cout << "Es hoja? " << (raiz->esHoja() ? "Sí" : "No") << "\n";
    std::cout << "Es solución entera? " << (raiz->esSolucionEntera() ? "No" : "Sí (esto sería raro!)") << "\n";

    // === 4. Verificar ramificación ===
    int varFrac = raiz->obtenerVarMasFraccionaria();
    std::cout << "Variable más fraccionaria: " << varFrac << "\n";

    // === 5. Verificar jerarquía de nodos ===
    auto hijo = std::make_shared<NodoArbol>(raiz);  // Crear hijo con padre raiz
    raiz->agregarHijo(hijo);
    std::cout << "Número de hijos de raíz: " << raiz->obtenerNumHijos() << "\n";

    // === 6. Imprimir información completa del nodo ===
    raiz->imprimir();

    return 0;
}
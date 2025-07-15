#include "NodoArbol.h"
#include <iostream>
#include <algorithm>
#include <cmath>

/**
 * Desc: Constructor por defecto.
 *       Inicializa un nodo con valores por defecto:
 *       profundidad = 0, cota inferior = infinito, factible = false.
 * Params: void
 * Returns: void
 */
NodoArbol::NodoArbol() : cotaInferior(0.0), cotaSuperior(1e9), esFactible(true), profundidad(0) {}

/**
 * Desc: Constructor que crea un nuevo nodo hijo a partir de un nodo padre.
 *       Copia la solución y variables fijadas del padre.
 * Params:
 *   - nodoPadre: Puntero compartido al nodo padre.
 * Returns: void
 */
NodoArbol::NodoArbol(shared_ptr<NodoArbol> nodoPadre) : padre(nodoPadre), cotaInferior(0.0), cotaSuperior(1e9), esFactible(true) {
    profundidad = nodoPadre ? nodoPadre->profundidad + 1 : 0;
    if (nodoPadre) {
        variablesFijadas = nodoPadre->variablesFijadas;
    }

    // cout << "[NodoArbol::NodoArbol(padre)] Nodo creado. Profundidad: " << profundidad << ", Variables fijadas: ";
    // for (const auto& [j, val] : variablesFijadas) {
    //     cout << "y" << j << "=" << val << " ";
    // }
    // cout << "\n";
}

/**
 * Desc: Añade un hijo a este nodo.
 * Params:
 *   - hijo: Puntero compartido al hijo a añadir.
 * Returns: void
 */
void NodoArbol::agregarHijo(shared_ptr<NodoArbol> hijo) {
    hijos.push_back(hijo);
}

/**
 * Desc: Elimina un hijo de este nodo.
 * Params:
 *   - hijo: Puntero compartido al hijo a eliminar.
 * Returns: void
 */
void NodoArbol::removerHijo(shared_ptr<NodoArbol> hijo) {
    auto it = find(hijos.begin(), hijos.end(), hijo);
    if (it != hijos.end()) {
        hijos.erase(it);
    }
}

/**
 * Desc: Devuelve un hijo específico por su índice.
 * Params:
 *   - indice: Índice del hijo en el vector.
 * Returns:
 *   - std::shared_ptr<NodoArbol>: Puntero al hijo solicitado.
 */
shared_ptr<NodoArbol> NodoArbol::obtenerHijo(int indice) const {
    if (indice >= 0 && indice < hijos.size()) {
        return hijos[indice];
    }
    return nullptr;
}

/**
 * Desc: Devuelve el número total de hijos que tiene este nodo.
 * Params: void
 * Returns:
 *   - int: Número de hijos.
 */
int NodoArbol::obtenerNumHijos() const {
    return hijos.size();
}

/**
 * Desc: Devuelve la cota inferior calculada para este nodo.
 * Params: void
 * Returns:
 *   - double: Valor de la cota inferior.
 */
double NodoArbol::obtenerCotaInferior() const { return cotaInferior; }

/**
 * Desc: Establece el valor de la cota inferior para este nodo.
 * Params:
 *   - cota: Nuevo valor de la cota inferior.
 * Returns: void
 */
void NodoArbol::establecerCotaInferior(double cota) { cotaInferior = cota; }

/**
 * Desc: Devuelve la cota superior estimada para este nodo.
 * Params: void
 * Returns:
 *   - double: Valor de la cota superior.
 */
double NodoArbol::obtenerCotaSuperior() const { return cotaSuperior; }

/**
 * Desc: Establece el valor de la cota superior para este nodo.
 * Params:
 *   - cota: Nuevo valor de la cota superior.
 * Returns: void
 */
void NodoArbol::establecerCotaSuperior(double cota) { cotaSuperior = cota; }

/**
 * Desc: Devuelve la solución continua asociada a este nodo.
 * Params: void
 * Returns:
 *   - const std::vector<double>&: Referencia a la solución.
 */
const vector<double>& NodoArbol::obtenerSolucion() const { return solucionContinua; }

/**
 * Desc: Establece una nueva solución continua para este nodo.
 * Params:
 *   - sol: Vector de dobles representando la solución.
 * Returns: void
 */
void NodoArbol::establecerSolucion(const vector<double>& sol) { solucionContinua = sol; }

/**
 * Desc: Devuelve el mapa de variables fijadas en este nodo.
 * Params: void
 * Returns:
 *   - const std::map<int, int>&: Referencia al mapa de variables fijadas.
 */
const map<int, int>& NodoArbol::obtenerVariablesFijadas() const { return variablesFijadas; }

/**
 * Desc: Fija el valor de una variable entera en este nodo.
 *       Usado durante la ramificación para forzar valores binarios (0 o 1).
 * Params:
 *   - var: Índice de la variable a fijar.
 *   - valor: Valor asignado (normalmente 0 o 1).
 * Returns: void
 */
void NodoArbol::fijarVariable(int var, double valor) {
    // Asegurar que el valor esté cerca de 0 o 1
    if (abs(valor - 0.0) < 1e-6) {
        variablesFijadas[var] = 0.0;
    } else if (abs(valor - 1.0) < 1e-6) {
        variablesFijadas[var] = 1.0;
    } else {
        variablesFijadas[var] = valor;
    }

    if (var >= 0 && var < solucionContinua.size()) {
        solucionContinua[var] = variablesFijadas[var];
    }
    
    // cout << "[NodoArbol::fijarVariable] Variable y" << var << " fijada a " << variablesFijadas[var] << "\n";
}

/**
 * Desc: Devuelve si este nodo es factible según la relajación LP.
 * Params: void
 * Returns:
 *   - bool: True si es factible, False en caso contrario.
 */
bool NodoArbol::obtenerFactibilidad() const { return esFactible; }

/**
 * Desc: Establece si este nodo es factible o no.
 * Params:
 *   - factible: Booleano indicando la factibilidad.
 * Returns: void
 */
void NodoArbol::establecerFactibilidad(bool factible) { esFactible = factible; }

/**
 * Desc: Devuelve la profundidad del nodo dentro del árbol de búsqueda.
 * Params: void
 * Returns:
 *   - int: Profundidad del nodo.
 */
int NodoArbol::obtenerProfundidad() const { return profundidad; }

/**
 * Desc: Establece manualmente la profundidad del nodo.
 * Params:
 *   - prof: Nuevo valor de profundidad.
 * Returns: void
 */
void NodoArbol::establecerProfundidad(int prof) { profundidad = prof; }

/**
 * Desc: Devuelve el puntero al nodo padre de este nodo.
 * Params: void
 * Returns:
 *   - std::shared_ptr<NodoArbol>: Puntero al nodo padre.
 */
shared_ptr<NodoArbol> NodoArbol::obtenerPadre() const { return padre; }

/**
 * Desc: Verifica si el nodo es una hoja del árbol de búsqueda.
 * Params: void
 * Returns:
 *   - bool: True si es hoja, False si tiene hijos.
 */
bool NodoArbol::esHoja() const { return hijos.empty(); }

/**
 * Desc: Verifica si la solución actual es entera (todas las variables son 0 o 1).
 *       Usa una tolerancia pequeña para considerar valores fraccionarios como enteros.
 * Params: void
 * Returns:
 *   - bool: True si la solución es entera, False en caso contrario.
 */
bool NodoArbol::esSolucionEntera() const {
    // cout << "[NodoArbol::esSolucionEntera] Verificando solución entera. Valores actuales:\n";
    // for (int i = 0; i < solucionContinua.size(); ++i) {
    //     cout << "  y" << i << " = " << solucionContinua[i] << " (redondeado: " << round(solucionContinua[i]) << ", diferencia: " << abs(solucionContinua[i] - round(solucionContinua[i])) << ")\n";
    // }
    
    for (double val : solucionContinua) {
        if (abs(val - round(val)) > 1e-6) {
            // cout << "  [Resultado] NO es solución entera\n";
            return false;
        }
    }
    // cout << "  [Resultado] ES solución entera\n";
    return true;
}

/**
 * Desc: Encuentra la variable más fraccionaria en la solución actual.
 *       Se usa durante la ramificación para seleccionar qué variable dividir.
 * Params: void
 * Returns:
 *   - int: Índice de la variable más fraccionaria.
 */
int NodoArbol::obtenerVarMasFraccionaria() const {
    int varMasFrac = -1;
    double maxFraccion = 0.0;

    // cout << "[NodoArbol::obtenerVarMasFraccionaria] Evaluando variables no fijadas:\n";
    
    for (int i = 0; i < solucionContinua.size(); i++) {
        if (variablesFijadas.find(i) != variablesFijadas.end()) {
            continue; // Variable ya fijada
        }
        
        double fraccion = abs(solucionContinua[i] - round(solucionContinua[i]));
        // cout << "  y" << i << " = " << solucionContinua[i] << " (fracción: " << fraccion << ")\n";
        
        if (fraccion > maxFraccion) {
            maxFraccion = fraccion;
            varMasFrac = i;
        }
    }

    // if (varMasFrac != -1) {
    //     cout << "  [Seleccionada] variable y" << varMasFrac << " con fracción " << maxFraccion << "\n";
    // } else {
    //     cout << "  [Error] No hay variables no fijadas para ramificar\n";
    // }
    
    return varMasFrac;
}

/**
 * Desc: Imprime información detallada sobre el nodo en consola.
 *       Usado principalmente para depuración.
 * Params: void
 * Returns: void
 */
void NodoArbol::imprimir() const {
    cout << "Nodo (profundidad: " << profundidad << ")\n";
    cout << "  Cota inferior: " << cotaInferior << "\n";
    cout << "  Cota superior: " << cotaSuperior << "\n";
    cout << "  Factible: " << (esFactible ? "Sí" : "No") << "\n";
    cout << "  Variables fijadas: ";
    for (const auto& [j, val] : variablesFijadas) {
        cout << "y" << j << "=" << val << " ";
    }
    cout << "\n";
    cout << "  Solución continua: ";
    for (double val : solucionContinua) {
        cout << val << " ";
    }
    cout << "\n";
}
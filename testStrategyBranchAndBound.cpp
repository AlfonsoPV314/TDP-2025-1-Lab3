#include "StrategyBranchAndBound.h"
#include "Problema.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/**
 * Desc: Crea un archivo de prueba con datos predefinidos para cargar un problema CFL.
 *       Contiene 2 fuentes, 3 clientes y datos de capacidades, costos y demandas.
 * Params:
 *   - nombreArchivo: Nombre del archivo a crear.
 * Returns: void
 */
void crearArchivoPrueba(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    // 2 fuentes, 3 clientes
    archivo << "2 3\n"; 
    // Capacidades de fuentes
    archivo << "100 200\n"; 
    // Costos de activación
    archivo << "10 20\n"; 
    // Demandas de clientes
    archivo << "50 60 70\n"; 
    // Matriz de costos de transporte (3x2)
    archivo << "5 6\n";   // Cliente 0: costos para fuente 0 y 1
    archivo << "7 8\n";   // Cliente 1: costos para fuente 0 y 1
    archivo << "9 10\n";  // Cliente 2: costos para fuente 0 y 1
    archivo.close();
}

int main() {
    // === 1. Crear archivo de prueba y cargar problema ===
    string archivo = "problema_test.txt";
    crearArchivoPrueba(archivo);
    Problema p(archivo);

    // === 2. Imprimir detalles del problema ===
    cout << "=== Test StrategyBranchAndBound ===\n";
    cout << "Número de clientes: " << p.getNumClientes() << "\n";
    cout << "Número de fuentes: " << p.getNumFuentesProd() << "\n";

    // === 3. Crear estrategia y resolver ===
    StrategyBranchAndBound strategy(p, 1000, 1e-6);
    ResultadoSolucion resultado = strategy.resolver(p);

    // === 4. Verificar resultados ===
    cout << "\n=== Resultados ===\n";
    cout << "Valor objetivo: " << resultado.valorObjetivo << "\n";
    cout << "Iteraciones: " << resultado.numIteraciones << "\n";
    cout << "Factible? " << (resultado.esFactible ? "Sí" : "No") << "\n";

    cout << "Solución final: ";
    for (double x : resultado.solucionFinal) {
        cout << x << " ";
    }
    cout << "\n";

    cout << "Tiempo de ejecución: " << resultado.tiempoEjecucion << " segundos\n";

    return 0;
}
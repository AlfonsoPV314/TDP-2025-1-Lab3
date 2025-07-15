#include "ResolveCFL.h"
#include "StrategyBranchAndBound.h"
#include "Problema.h"

using namespace std;

/**
 * Desc: Crea un archivo de prueba con un problema CFL predefinido.
 *       El archivo contiene 2 fuentes, 2 clientes, capacidades, costos de activación,
 *       demandas y matriz de costos de transporte.
 * Params:
 *   - nombreArchivo: Nombre del archivo a crear.
 * Returns: void
 */
void crearArchivoPrueba(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    // 2 fuentes, 2 clientes
    archivo << "2 2\n"; 
    // Capacidades de fuentes
    archivo << "100 100\n"; 
    // Costos de activación
    archivo << "10 15\n"; 
    // Demandas de clientes
    archivo << "50 50\n"; 
    // Matriz de costos de transporte
    archivo << "5 8\n";   // Cliente 0: costos para fuente 0 y 1
    archivo << "6 7\n";   // Cliente 1: costos para fuente 0 y 1
    archivo.close();
}

int main() {
    string archivo = "cfl_test.txt";
    crearArchivoPrueba(archivo);

    // Cargar el problema desde el archivo
    Problema p(archivo);
    
    // Imprimir detalles del problema (opcional para debug)
    p.imprimir();

    // Crear estrategia de resolución con parámetros personalizados
    auto strategy = make_shared<StrategyBranchAndBound>(p, 1000, 1e-6);
    ResolveCFL solver(strategy);

    // Mostrar nombre de la estrategia actual
    cout << "Estrategia actual: " << solver.getNombreStrategy() << "\n";

    // Resolver el problema
    auto resultado = solver.resolver(p);

    // Mostrar resultados
    cout << "\n=== Resultado ===\n";
    cout << "Valor objetivo: " << resultado.valorObjetivo << "\n";
    cout << "Iteraciones: " << resultado.numIteraciones << "\n";
    cout << "Factible? " << (resultado.esFactible ? "Sí" : "No") << "\n";

    cout << "Solución encontrada:\n";
    for (double x : resultado.solucionFinal) {
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}
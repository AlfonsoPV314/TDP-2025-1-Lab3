#include "Problema.h"       
#include <iostream>         
#include <fstream>          

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
    // === 1. Crear archivo de prueba ===
    string archivo = "problema_test.txt";
    crearArchivoPrueba(archivo);

    // === 2. Cargar problema desde archivo ===
    Problema p(archivo);

    // === 3. Verificar getters básicos ===
    cout << "Número de clientes: " << p.getNumClientes() << "\n";
    cout << "Número de fuentes: " << p.getNumFuentesProd() << "\n";
    cout << "Costo envío cliente 1 a fuente 1: " << p.getCostoEnvio(1, 1) << "\n";
    cout << "Costo activación fuente 1: " << p.getCostoActivacion(1) << "\n";
    cout << "Demanda cliente 0: " << p.getDemanda(0) << "\n";
    cout << "Capacidad fuente 0: " << p.getCapacidad(0) << "\n";

    // === 4. Verificar utilidades ===
    cout << "Es válido? " << (p.esValido() ? "Sí" : "No") << "\n";

    // === 5. Imprimir información completa del problema ===
    cout << "\n== Imprimir problema ==\n";
    p.imprimir();

    return 0;
}
#include "Problema.h"

/**
 * Desc: Constructor que carga los datos del problema desde un archivo.
 *       Inicializa estructuras como matrices de costos, capacidades y demandas.
 * Params:
 *   - nombreArchivo: Nombre del archivo de entrada con los datos del problema.
 * Returns: void
 */
Problema::Problema(const string& nombreArchivo) {

    // cout << "[Problema::Problema] Cargando problema desde: " << nombreArchivo << "\n";
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
    }

    // Leer número de fuentes y clientes
    archivo >> numFuentesProd >> numClientes;
    // cout << "[Problema::Problema] Fuentes: " << numFuentesProd << ", Clientes: " << numClientes << "\n";

    // Inicializar matrices
    matrizCostos.resize(numClientes, vector<double>(numFuentesProd));
    costosFuenteProd.resize(numFuentesProd);
    demandasClientes.resize(numClientes);
    capacidadesFuenteProd.resize(numFuentesProd);

    // Leer capacidades de fuentes
    for (int j = 0; j < numFuentesProd; j++) {
        archivo >> capacidadesFuenteProd[j];
    }

    // cout << "[Problema::Problema] Capacidades: ";
    // for (int j = 0; j < numFuentesProd; j++) {
    //     cout << capacidadesFuenteProd[j] << " ";
    // }
    // cout << "\n";

    // Leer costos de activación
    for (int j = 0; j < numFuentesProd; j++) {
        archivo >> costosFuenteProd[j];
    }

    // cout << "[Problema::Problema] Costos de activación: ";
    // for (int j = 0; j < numFuentesProd; j++) {
    //     cout << costosFuenteProd[j] << " ";
    // }
    // cout << "\n";

    // Leer demandas
    for (int i = 0; i < numClientes; i++) {
        archivo >> demandasClientes[i];
    }

    // cout << "[Problema::Problema] Demandas: ";
    // for (int i = 0; i < numClientes; i++) {
    //     cout << demandasClientes[i] << " ";
    // }
    // cout << "\n";

    // Leer matriz de costos de envío
    for (int i = 0; i < numClientes; i++) {
        for (int j = 0; j < numFuentesProd; j++) {
            archivo >> matrizCostos[i][j];
        }
    }
    // cout << "[Problema::Problema] Matriz de costos cargada\n";
    archivo.close();
}

/**
 * Desc: Imprime toda la información del problema en consola para depuración.
 *       Muestra número de clientes, fuentes, costos, capacidades y demandas.
 * Params: void
 * Returns: void
 */
void Problema::imprimir() const {
    cout << "Problema CFL:\n";
    cout << "Fuentes de producción: " << numFuentesProd << ", Clientes: " << numClientes << "\n";
    cout << "Capacidades: ";
    for (int j = 0; j < numFuentesProd; j++) {
        cout << capacidadesFuenteProd[j] << " ";
    }
    cout << "\n";
    cout << "Costos de activación: ";
    for (int j = 0; j < numFuentesProd; j++) {
        cout << costosFuenteProd[j] << " ";
    }
    cout << "\n";
    cout << "Demandas: ";
    for (int i = 0; i < numClientes; i++) {
        cout << demandasClientes[i] << " ";
    }
    cout << "\n";
    cout << "Matriz de costos de envío:\n";
    for (int i = 0; i < numClientes; i++) {
        for (int j = 0; j < numFuentesProd; j++) {
            cout << matrizCostos[i][j] << " ";
        }
        cout << "\n";
    }
}

/**
 * Desc: Verifica si el problema es válido (tiene suficiente capacidad global).
 *       Compara la capacidad total con la demanda total.
 * Params: void
 * Returns:
 *   - bool: True si el problema es factible, False en caso contrario.
 */
bool Problema::esValido() const {
    return numClientes > 0 && numFuentesProd > 0 && 
            matrizCostos.size() == numClientes && 
            costosFuenteProd.size() == numFuentesProd && 
            demandasClientes.size() == numClientes && 
            capacidadesFuenteProd.size() == numFuentesProd;
}

/**
 * Desc: Devuelve el número total de clientes en el problema.
 * Params: void
 * Returns:
 *   - int: Número de clientes.
 */
int Problema::getNumClientes() const { return numClientes; }

/**
 * Desc: Devuelve el número total de fuentes de producción (plantas).
 * Params: void
 * Returns:
 *   - int: Número de fuentes de producción.
 */
int Problema::getNumFuentesProd() const { return numFuentesProd; }

/**
 * Desc: Devuelve el costo de enviar desde una fuente a un cliente específico.
 * Params:
 *   - cliente: Índice del cliente.
 *   - fuenteProd: Índice de la fuente de producción.
 * Returns:
 *   - double: Costo de envío desde la fuente al cliente.
 */
double Problema::getCostoEnvio(int cliente, int fuenteProd) const { return matrizCostos[cliente][fuenteProd]; }

/**
 * Desc: Devuelve el costo de activar una fuente de producción específica.
 * Params:
 *   - fuenteProd: Índice de la fuente de producción.
 * Returns:
 *   - double: Costo de activación de la fuente.
 */
double Problema::getCostoActivacion(int fuenteProd) const { return costosFuenteProd[fuenteProd]; }

/**
 * Desc: Devuelve la demanda de un cliente específico.
 * Params:
 *   - cliente: Índice del cliente.
 * Returns:
 *   - double: Demanda del cliente.
 */
double Problema::getDemanda(int cliente) const { return demandasClientes[cliente]; }

/**
 * Desc: Devuelve la capacidad máxima de una fuente de producción específica.
 * Params:
 *   - fuenteProd: Índice de la fuente de producción.
 * Returns:
 *   - double: Capacidad de la fuente.
 */
double Problema::getCapacidad(int fuenteProd) const { return capacidadesFuenteProd[fuenteProd]; }

/**
 * Desc: Devuelve el vector completo con las capacidades de todas las fuentes.
 * Params: void
 * Returns:
 *   - const vector<double>&: Referencia al vector de capacidades.
 */
const vector<double>& Problema::getCapacidades() const {
    return capacidadesFuenteProd;
}

/**
 * Desc: Devuelve el vector completo con las demandas de todos los clientes.
 * Params: void
 * Returns:
 *   - const vector<double>&: Referencia al vector de demandas.
 */
const vector<double>& Problema::getDemandas() const {
    return demandasClientes;
}
#include "Problema.h"
#include "ResolveCFL.h"
#include "StrategyBranchAndBound.h"
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <limits>
#include <iomanip>

void mostrarBanner() {
    cout << "\n";
    cout << "================================================\n";
    cout << "    SOLUCIONADOR DE PROBLEMAS CFL\n";
    cout << "    (Capacitated Facility Location)\n";
    cout << "================================================\n";
    cout << "\n";
}

void mostrarMenu() {
    cout << "\n--- MENÚ PRINCIPAL ---\n";
    cout << "1. Cargar archivo de problema\n";
    cout << "2. Mostrar información del problema actual\n";
    cout << "3. Resolver problema con Branch and Bound\n";
    cout << "4. Mostrar formato de archivo de entrada\n";
    cout << "5. Crear archivo de ejemplo\n";
    cout << "0. Salir\n";
    cout << "\nIngrese su opción: ";
}

void mostrarFormatoArchivo() {
    cout << "\n--- FORMATO DE ARCHIVO DE ENTRADA ---\n";
    cout << "Línea 1: m n (número de fuentes, número de clientes)\n";
    cout << "Línea 2: M1 M2 ... Mm (capacidades de fuentes)\n";
    cout << "Línea 3: f1 f2 ... fm (costos de activación de fuentes)\n";
    cout << "Línea 4: d1 d2 ... dn (demandas de clientes)\n";
    cout << "Líneas 5 a n+4: matriz de costos de envío c[i][j]\n";
    cout << "  donde c[i][j] es el costo de envío del cliente i a la fuente j\n";
    cout << "\nEjemplo para 2 fuentes y 3 clientes:\n";
    cout << "2 3\n";
    cout << "100 150\n";
    cout << "50 80\n";
    cout << "30 40 20\n";
    cout << "10 15\n";
    cout << "12 8\n";
    cout << "5 20\n";
}

void crearArchivoEjemplo() {
    string nombreArchivo;
    cout << "\nIngrese el nombre del archivo a crear (ej: ejemplo.txt): ";
    cin >> nombreArchivo;
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo crear el archivo " << nombreArchivo << "\n";
        return;
    }
    archivo << "3 4\n";                    // 3 fuentes, 4 clientes
    archivo << "100 120 80\n";             // Capacidades
    archivo << "50 60 40\n";               // Costos de activación
    archivo << "25 30 20 35\n";            // Demandas
    archivo << "10 15 12\n";               // Costos cliente 0
    archivo << "8 20 18\n";                // Costos cliente 1
    archivo << "12 10 25\n";               // Costos cliente 2
    archivo << "15 8 14\n";                // Costos cliente 3
    archivo.close();
    cout << "Archivo de ejemplo creado exitosamente: " << nombreArchivo << "\n";
    cout << "Puede usar este archivo para probar el programa.\n";
}

bool verificarArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << "\n";
        return false;
    }
    int m, n;
    if (!(archivo >> m >> n)) {
        cout << "Error: No se pudieron leer las dimensiones del problema\n";
        archivo.close();
        return false;
    }
    if (m <= 0 || n <= 0) {
        cout << "Error: Las dimensiones deben ser positivas\n";
        archivo.close();
        return false;
    }
    archivo.close();
    return true;
}

void mostrarResultados(const ResultadoSolucion& resultado, const string& nombreStrategy) {
    cout << "\n--- RESULTADOS DE LA RESOLUCIÓN ---\n";
    cout << "Estrategia utilizada: " << nombreStrategy << "\n";
    cout << "Factibilidad: " << (resultado.esFactible ? "FACTIBLE" : "NO FACTIBLE") << "\n";
    if (resultado.esFactible) {
        cout << fixed << setprecision(2);
        cout << "Valor objetivo: " << resultado.valorObjetivo << "\n";
        cout << "Número de iteraciones: " << resultado.numIteraciones << "\n";
        cout << "Tiempo de ejecución: " << resultado.tiempoEjecucion << " segundos\n";
        if (!resultado.solucionFinal.empty()) {
            cout << "\nSolución encontrada:\n";
            cout << "Fuentes activadas: ";
            for (int j = 0; j < resultado.solucionFinal.size(); j++) {
                if (resultado.solucionFinal[j] > 0.5) {
                    cout << "F" << j << " ";
                }
            }
            cout << "\n";
            cout << "Estado de todas las fuentes:\n";
            for (int j = 0; j < resultado.solucionFinal.size(); j++) {
                cout << "  Fuente " << j << ": " << 
                    (resultado.solucionFinal[j] > 0.5 ? "ACTIVA" : "INACTIVA") << 
                    " (valor: " << resultado.solucionFinal[j] << ")\n";
            }
        }
    } else {
        cout << "No se encontró una solución factible para el problema.\n";
    }
}

int leerOpcion() {
    int opcion;
    while (!(cin >> opcion)) {
        cout << "Por favor, ingrese un número válido: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return opcion;
}

string leerNombreArchivo() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo: ";
    getline(cin, nombreArchivo);
    return nombreArchivo;
}

int main() {
    mostrarBanner();
    shared_ptr<Problema> problemaActual = nullptr;
    string nombreArchivoActual = "";
    int opcion;

    do {
        mostrarMenu();
        opcion = leerOpcion();

        switch (opcion) {
            case 1: {
                string nombreArchivo = leerNombreArchivo();
                if (verificarArchivo(nombreArchivo)) {
                    try {
                        problemaActual = make_shared<Problema>(nombreArchivo);
                        nombreArchivoActual = nombreArchivo;
                        cout << "\n¡Problema cargado exitosamente desde: " << nombreArchivo << "!\n";
                        if (problemaActual->esValido()) {
                            cout << "Validación: El problema es válido.\n";
                            cout << "Fuentes: " << problemaActual->getNumFuentesProd() << ", Clientes: " << problemaActual->getNumClientes() << "\n";
                        } else {
                            cout << "Advertencia: El problema puede tener inconsistencias.\n";
                        }
                    } catch (const exception& e) {
                        cout << "Error al cargar el problema: " << e.what() << "\n";
                        problemaActual = nullptr;
                    }
                }
                break;
            }
            case 2: {
                if (!problemaActual) {
                    cout << "\nNo hay ningún problema cargado. Use la opción 1 para cargar un archivo.\n";
                } else {
                    cout << "\n--- INFORMACIÓN DEL PROBLEMA ---\n";
                    cout << "Archivo: " << nombreArchivoActual << "\n";
                    problemaActual->imprimir();

                    // Calcular demanda total
                    double demandaTotal = 0;
                    for (int i = 0; i < problemaActual->getNumClientes(); i++) {
                        demandaTotal += problemaActual->getDemanda(i);
                    }

                    // Calcular capacidad total usando getCapacidades()
                    double capacidadTotal = 0;
                    const auto& capacidades = problemaActual->getCapacidades();
                    for (double cap : capacidades) {
                        capacidadTotal += cap;
                    }

                    cout << "\nResumen:\n";
                    cout << "Demanda total: " << demandaTotal << "\n";
                    cout << "Capacidad total: " << capacidadTotal << "\n";
                    cout << "Ratio capacidad/demanda: " << (capacidadTotal / demandaTotal) << "\n";
                }
                break;
            }
            case 3: {
                if (!problemaActual) {
                    cout << "\nNo hay ningún problema cargado. Use la opción 1 para cargar un archivo.\n";
                } else {
                    cout << "\n--- INICIANDO RESOLUCIÓN ---\n";
                    cout << "Resolviendo problema con Branch and Bound...\n";

                    int maxIteraciones = 10000;
                    double tolerancia = 1e-6;

                    cout << "¿Desea cambiar los parámetros? (s/n): ";
                    char respuesta;
                    cin >> respuesta;

                    if (respuesta == 's' || respuesta == 'S') {
                        cout << "Máximo número de iteraciones [" << maxIteraciones << "]: ";
                        int nuevoMax;
                        if (cin >> nuevoMax && nuevoMax > 0) {
                            maxIteraciones = nuevoMax;
                        }
                        cout << "Tolerancia [" << tolerancia << "]: ";
                        double nuevaTol;
                        if (cin >> nuevaTol && nuevaTol > 0) {
                            tolerancia = nuevaTol;
                        }
                    }

                    auto estrategia = make_shared<StrategyBranchAndBound>(*problemaActual, maxIteraciones, tolerancia);
                    ResolveCFL resolvedor(estrategia);
                    cout << "\nEjecutando algoritmo...\n";
                    ResultadoSolucion resultado = resolvedor.resolver(*problemaActual);
                    mostrarResultados(resultado, resolvedor.getNombreStrategy());
                }
                break;
            }
            case 4: {
                mostrarFormatoArchivo();
                break;
            }
            case 5: {
                crearArchivoEjemplo();
                break;
            }
            case 0: {
                cout << "\nchao pescao\n";
                break;
            }
            default: {
                cout << "\nOpción no válida. Por favor, seleccione una opción del 0 al 5.\n";
                break;
            }
        }

        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }

    } while (opcion != 0);

    return 0;
}
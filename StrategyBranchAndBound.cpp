#include "StrategyBranchAndBound.h"

/**
 * Desc: Constructor de la estrategia Branch and Bound.
 *       Inicializa variables como demanda total, número máximo de iteraciones y tolerancia.
 * Params:
 *   - problema: Referencia constante al problema a resolver.
 *   - maxIter: Número máximo de iteraciones permitidas.
 *   - tol: Tolerancia para determinar la optimalidad.
 * Returns: void
 */
StrategyBranchAndBound::StrategyBranchAndBound(const Problema& problema, int maxIter, double tol) 
    : maxIteraciones(maxIter), tolerancia(tol) {
    demandaTotal = 0.0;
    for (int i = 0; i < problema.getNumClientes(); i++) {
        demandaTotal += problema.getDemanda(i);
    }
}

/**
 * Desc: Resuelve el problema utilizando Branch and Bound.
 *       Implementa la búsqueda en profundidad guiada por cotas inferiores y superiores.
 * Params:
 *   - problema: Referencia constante al problema a resolver.
 * Returns:
 *   - ResultadoSolucion: Estructura que contiene la solución óptima encontrada,
 *                         incluyendo costo, tiempo de ejecución y factibilidad.
 */
ResultadoSolucion StrategyBranchAndBound::resolver(const Problema& problema) {
    auto tiempoInicio = chrono::high_resolution_clock::now();
    ResultadoSolucion resultado;
    resultado.numIteraciones = 0;
    resultado.esFactible = false;

    priority_queue<shared_ptr<NodoArbol>, vector<shared_ptr<NodoArbol>>, ComparadorNodos> colaNodos;
    
    // Solución greedy inicial
    auto solGreedy = getSolucionGreedy(problema);
    double mejorCotaSup = calcularCostoExacto(problema, solGreedy);
    vector<double> mejorSolucion = solGreedy;
    cout << "Solución greedy inicial - Costo: " << mejorCotaSup << "\n";

    // Nodo raíz
    auto nodoRaiz = make_shared<NodoArbol>();
    resolverRelajacionLP(problema, nodoRaiz);

    // cout << "[StrategyBranchAndBound::resolver] Raíz - Cota inferior: " << nodoRaiz->obtenerCotaInferior() << ", Factible: " << (nodoRaiz->obtenerFactibilidad() ? "Sí" : "No") << "\n";

    if (!nodoRaiz->obtenerFactibilidad()) {
        // cout << "[StrategyBranchAndBound::resolver] Nodo raíz inviable, terminando\n";
        return resultado;
    }
    colaNodos.push(nodoRaiz);
    double mejorCotaInf = nodoRaiz->obtenerCotaInferior();

    while (!colaNodos.empty() && resultado.numIteraciones < maxIteraciones) {
        resultado.numIteraciones++;
        auto nodoActual = colaNodos.top();
        colaNodos.pop();
    
        // Poda por cota
        if (nodoActual->obtenerCotaInferior() >= mejorCotaSup + tolerancia) {
            continue;
        }
    
        // cout << "[StrategyBranchAndBound::resolver] Procesando nodo - Cota: " << nodoActual->obtenerCotaInferior() << "\n";
    
        // Verificar si es solución entera
        if (nodoActual->esSolucionEntera()) {
            double costoReal = calcularCostoExacto(problema, nodoActual->obtenerSolucion());
            if (costoReal <= mejorCotaSup + tolerancia && costoReal >= 0) {
                mejorCotaSup = costoReal;
                mejorSolucion = nodoActual->obtenerSolucion();
                resultado.esFactible = true;
                // cout << "[StrategyBranchAndBound::resolver] Nueva mejor solución: " << mejorCotaSup << "\n";
            }
            continue;
        }
    
        // Ramificación
        int varIdx = nodoActual->obtenerVarMasFraccionaria();
        // cout << "[StrategyBranchAndBound::resolver] Ramificando en variable y" << varIdx << "\n";
    
        // Crear hijos y resolver relajación LP
        for (int valor : {0, 1}) {
            auto nuevoNodo = make_shared<NodoArbol>(*nodoActual);
            nuevoNodo->fijarVariable(varIdx, valor);
            resolverRelajacionLP(problema, nuevoNodo);
    
            if (nuevoNodo->obtenerFactibilidad() && 
                nuevoNodo->obtenerCotaInferior() <= mejorCotaSup + tolerancia) {
                colaNodos.push(nuevoNodo);
            }
        }
    
        // Actualizar mejor cota inferior
        if (!colaNodos.empty()) {
            mejorCotaInf = colaNodos.top()->obtenerCotaInferior();
        }
    
        // Verificar optimalidad al final
        if (abs(mejorCotaSup - mejorCotaInf) <= tolerancia) {
            // Procesar último nodo para confirmar
            if (nodoActual->esSolucionEntera() && nodoActual->obtenerFactibilidad()) {
                double costoReal = calcularCostoExacto(problema, nodoActual->obtenerSolucion());
                if (costoReal <= mejorCotaSup + tolerancia) {
                    mejorCotaSup = costoReal;
                    mejorSolucion = nodoActual->obtenerSolucion();
                    resultado.esFactible = true;
                    // cout << "[StrategyBranchAndBound::resolver] Solución óptima confirmada - Costo: " << mejorCotaSup << "\n";
                }
            }
            break;
        }
    }

    // Configurar resultado final
    if (resultado.esFactible) {
        resultado.solucionFinal = mejorSolucion;
        resultado.valorObjetivo = mejorCotaSup;
    }
    auto tiempoFin = chrono::high_resolution_clock::now();
    resultado.tiempoEjecucion = chrono::duration<double>(tiempoFin - tiempoInicio).count();
    // cout << "[StrategyBranchAndBound::resolver] Finalizando - Solución factible: " << (resultado.esFactible ? "Sí" : "No") << ", Costo: " << resultado.valorObjetivo << "\n";
    return resultado;
}

/**
 * Desc: Resuelve la relajación LP de un nodo dado usando simplex de COIN-OR.
 *       Calcula la cota inferior y verifica la factibilidad del nodo.
 * Params:
 *   - problema: Referencia constante al problema a resolver.
 *   - nodo: Puntero compartido al nodo actual.
 * Returns: void
 */
void StrategyBranchAndBound::resolverRelajacionLP(const Problema& problema, shared_ptr<NodoArbol> nodo) {
    const auto& fijadas = nodo->obtenerVariablesFijadas();
    int numCli = problema.getNumClientes();
    int numFue = problema.getNumFuentesProd();
    
    // Crear modelo de COIN-OR usando ClpSimplex
    ClpSimplex modelo;
    
    // Número de variables: x_ij (transporte) + y_j (activación)
    int numVarX = numCli * numFue;
    int numVarY = numFue;
    int totalVars = numVarX + numVarY;
    
    // Límites inferiores y superiores para variables
    double* lower = new double[totalVars];
    double* upper = new double[totalVars];
    
    // Variables x_ij (transporte): [0, inf)
    for (int i = 0; i < numVarX; ++i) {
        lower[i] = 0.0;
        upper[i] = COIN_DBL_MAX;
    }
    // Variables y_j (activación): [0, 1] o fijadas si están en el nodo
    for (int j = 0; j < numFue; ++j) {
        int idx = numVarX + j;
        auto it = fijadas.find(j);
        if (it != fijadas.end()) {
            lower[idx] = upper[idx] = it->second; // Fijada
        } else {
            lower[idx] = 0.0;
            upper[idx] = 1.0; // LP relajado
        }
    }
    
    // Función objetivo: min sum(f_j * y_j) + sum(c_ij * x_ij)
    double* objCoeffs = new double[totalVars];
    for (int j = 0; j < numFue; ++j) {
        objCoeffs[numVarX + j] = problema.getCostoActivacion(j);
    }
    for (int i = 0; i < numCli; ++i) {
        for (int j = 0; j < numFue; ++j) {
            objCoeffs[i*numFue + j] = problema.getCostoEnvio(i, j);
        }
    }
    
    // Construir matriz de restricciones usando CoinPackedMatrix
    CoinPackedMatrix matrix;
    matrix.setDimensions(0, totalVars);
    
    // Restricciones de demanda: sum_j x_ij = d_i
    for (int i = 0; i < numCli; ++i) {
        CoinPackedVector fila;
        for (int j = 0; j < numFue; ++j) {
            fila.insert(i*numFue + j, 1.0);
        }
        matrix.appendRow(fila);
    }
    
    // Restricciones de capacidad: sum_i x_ij - M_j * y_j <= 0
    for (int j = 0; j < numFue; ++j) {
        CoinPackedVector fila;
        for (int i = 0; i < numCli; ++i) {
            fila.insert(i*numFue + j, 1.0);
        }
        fila.insert(numVarX + j, -problema.getCapacidad(j)); // -M_j * y_j
        matrix.appendRow(fila);
    }
    
    // Límites de las restricciones
    double* rowLower = new double[numCli + numFue];
    double* rowUpper = new double[numCli + numFue];
    
    for (int i = 0; i < numCli; ++i) {
        rowLower[i] = rowUpper[i] = problema.getDemanda(i);
    }
    for (int j = 0; j < numFue; ++j) {
        rowLower[numCli + j] = -COIN_DBL_MAX;
        rowUpper[numCli + j] = 0.0;
    }
    
    modelo.loadProblem(matrix, lower, upper, objCoeffs, rowLower, rowUpper);
    
    // Configurar el modelo
    modelo.dual();
    
    double cotaInf = modelo.objectiveValue();
    const double* solucion = modelo.primalColumnSolution();
    
    // Verificar factibilidad (considerando tolerancia numérica)
    bool factible = modelo.status() == 0;
    vector<double> solucionY(numFue);
    for (int j = 0; j < numFue; ++j) {
        solucionY[j] = solucion[numVarX + j];
    }
    
    if (factible) {
        nodo->establecerSolucion(solucionY);
        nodo->establecerCotaInferior(cotaInf);
        nodo->establecerFactibilidad(true);
    } else {
        nodo->establecerFactibilidad(false);
    }
    
    delete[] lower;
    delete[] upper;
    delete[] objCoeffs;
    delete[] rowLower;
    delete[] rowUpper;
}

/**
 * Desc: Calcula el costo exacto de una solución entera dada.
 *       Verifica si la solución es factible y asigna clientes a fuentes de producción.
 * Params:
 *   - problema: Referencia constante al problema a resolver.
 *   - solucion: Vector de dobles representando la solución entera (activación de fuentes).
 * Returns:
 *   - double: Costo total de la solución (o 1e20 si no es factible).
 */
double StrategyBranchAndBound::calcularCostoExacto(const Problema& problema, const vector<double>& solucion) {

    // cout << "[StrategyBranchAndBound::calcularCostoExacto] Calculando costo exacto\n";
    // cout << "[StrategyBranchAndBound::calcularCostoExacto] Solución: ";
    // for (int j = 0; j < solucion.size(); j++) {
    //     cout << "y" << j << "=" << solucion[j] << " ";
    // }
    // cout << endl;

    double capacidadTotal = 0.0;
    for (int j = 0; j < solucion.size(); j++) {
        if (solucion[j] > 0.5) {
            capacidadTotal += problema.getCapacidad(j);
        }
    }
    // cout << "[StrategyBranchAndBound::calcularCostoExacto] Capacidad total: " << capacidadTotal << ", Demanda total: " << demandaTotal << "\n";
    
    if (capacidadTotal < demandaTotal) {
        // cout << "[StrategyBranchAndBound::calcularCostoExacto] Solución inviable - Capacidad insuficiente\n";
        return 1e20;
    }
    
    vector<double> capRestante = problema.getCapacidades();
    vector<double> demRestante = problema.getDemandas();  // Asumiendo que existe esta función
    double costoTotal = 0.0;
    
    // Costos fijos
    for (int j = 0; j < solucion.size(); j++) {
        if (solucion[j] > 0.5) {
            costoTotal += problema.getCostoActivacion(j);
        }
    }
    
    // Asignar clientes considerando eficiencia costo/demanda
    vector<tuple<double, int>> clientesPorDensidad;
    for (int i = 0; i < problema.getNumClientes(); i++) {
        double mejorCosto = 1e20;
        for (int j = 0; j < problema.getNumFuentesProd(); j++) {
            if (solucion[j] > 0.5) {
                mejorCosto = min(mejorCosto, problema.getCostoEnvio(i, j));
            }
        }
        if (mejorCosto < 1e20) {
            double densidad = mejorCosto / demRestante[i];
            clientesPorDensidad.emplace_back(densidad, i);
        }
    }
    
    sort(clientesPorDensidad.begin(), clientesPorDensidad.end());
    
    for (const auto& [densidad, idxCliente] : clientesPorDensidad) {
        int cliente = idxCliente;
        double dem = demRestante[cliente];
        double mejorCosto = 1e20;
        int mejorFacilidad = -1;
        
        vector<tuple<double, int, double>> fuentesPorEficiencia;
        for (int j = 0; j < problema.getNumFuentesProd(); j++) {
            if (solucion[j] > 0.5 && capRestante[j] > 1e-6) {
                double costo = problema.getCostoEnvio(cliente, j);
                double eficiencia = costo / capRestante[j];
                fuentesPorEficiencia.emplace_back(costo, j, eficiencia);
            }
        }
        
        // en el principio, solo Dios y yo sabíamos lo que hacía el código. ahora, solo Dios lo sabe
        sort(fuentesPorEficiencia.begin(), fuentesPorEficiencia.end(), [](const auto& a, const auto& b) { return get<2>(a) < get<2>(b);});
        
        if (!fuentesPorEficiencia.empty()) {
            mejorFacilidad = get<1>(fuentesPorEficiencia[0]);
            mejorCosto = get<0>(fuentesPorEficiencia[0]);
        }
        
        if (mejorFacilidad == -1) {
            return 1e20;
        }
        
        costoTotal += mejorCosto * dem;
        capRestante[mejorFacilidad] -= dem;
    }

    // cout << "[StrategyBranchAndBound::calcularCostoExacto] Solución factible - Costo total: " << costoTotal << "\n";
    
    return costoTotal;
}

/**
 * Desc: Filtra nodos cuya cota inferior es peor que la cota superior actual.
 *       Reduce el tamaño del árbol de búsqueda eliminando ramas inviables.
 * Params:
 *   - mejorCotaSup: Cota superior actual (mejor solución encontrada hasta ahora).
 * Returns: void
 */
void StrategyBranchAndBound::podarNodos(double mejorCotaSup) {

    priority_queue<shared_ptr<NodoArbol>, vector<shared_ptr<NodoArbol>>, ComparadorNodos> nuevaCola;
    while (!colaNodos.empty()) {
        auto nodo = colaNodos.top();
        colaNodos.pop();
        if (nodo->obtenerCotaInferior() <= mejorCotaSup + tolerancia) {
            nuevaCola.push(nodo);
            // cout << "[StrategyBranchAndBound::resolver] Nodo hijo añadido - Cota: " << nodo->obtenerCotaInferior() << "\n";
        }//else{
        //     cout << "[StrategyBranchAndBound::resolver] Nodo hijo rechazado - Cota: " << nodo->obtenerCotaInferior() << "\n";
        // }
    }
    colaNodos = move(nuevaCola);
}

/**
 * Desc: Verifica si la solución actual es óptima dentro de la tolerancia dada.
 * Params:
 *   - cotaInf: Cota inferior del nodo actual.
 *   - cotaSup: Cota superior (mejor solución encontrada).
 * Returns:
 *   - bool: True si la solución es óptima, False en caso contrario.
 */
bool StrategyBranchAndBound::esOptimo(double cotaInf, double cotaSup) const {
    return abs(cotaSup - cotaInf) <= tolerancia;
}

/**
 * Desc: Genera una solución inicial greedy para el problema.
 *       Asigna fuentes de producción basándose en métricas de costo/capacidad.
 * Params:
 *   - problema: Referencia constante al problema a resolver.
 * Returns:
 *   - vector<double>: Solución greedy (activación de fuentes).
 */
vector<double> StrategyBranchAndBound::getSolucionGreedy(const Problema& problema) {
    int numFuentesProd = problema.getNumFuentesProd();
    int numClientes = problema.getNumClientes();
    vector<int> solucion(numFuentesProd, 0);
    double capTotal = 0.0;
    
    // Calcular métrica mejorada considerando costos de transporte reales
    vector<tuple<double, int>> metricas;
    
    for (int j = 0; j < numFuentesProd; ++j) {
        double costoAct = problema.getCostoActivacion(j);
        double cap = problema.getCapacidad(j);
        
        double transporteOptimizado = 0.0;
        double capDisponible = cap;
        
        // Calcular costo de transporte optimizado por cliente
        vector<tuple<double, double, int>> clientesPorDensidad;
        for (int i = 0; i < numClientes; ++i) {
            double costo = problema.getCostoEnvio(i, j);
            double dem = problema.getDemanda(i);
            double densidad = costo / dem;
            clientesPorDensidad.emplace_back(densidad, dem, i);
        }
        sort(clientesPorDensidad.begin(), clientesPorDensidad.end());
        
        // Asignar clientes por orden de eficiencia
        for (const auto& [densidad, dem, idx] : clientesPorDensidad) {
            if (dem <= capDisponible) {
                transporteOptimizado += dem * problema.getCostoEnvio(idx, j);
                capDisponible -= dem;
            }
        }
        
        double metrica = (costoAct + transporteOptimizado) / cap;
        metricas.emplace_back(metrica, j);
    }
    
    sort(metricas.begin(), metricas.end(), [](const auto& a, const auto& b) {
        return get<0>(a) < get<0>(b);
    });
    
    for (const auto& [m, j] : metricas) {
        if (capTotal >= demandaTotal - tolerancia) break;
        if (solucion[j] == 0) {
            solucion[j] = 1;
            capTotal += problema.getCapacidad(j);
        }
    }
    
    vector<double> solucionDouble(numFuentesProd);
    for (int j = 0; j < numFuentesProd; ++j) {
        solucionDouble[j] = static_cast<double>(solucion[j]);
    }
    return solucionDouble;
}
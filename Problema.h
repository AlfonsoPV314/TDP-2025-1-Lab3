#ifndef PROBLEMA_H
#define PROBLEMA_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

class Problema {
private:
    int numClientes;
    int numFuentesProd;
    vector<vector<double>> matrizCostos; // c[i][j]
    vector<double> costosFuenteProd; // f[j]
    vector<double> demandasClientes; // d[i]
    vector<double> capacidadesFuenteProd; // M[j]

public:
    Problema(const string& nombreArchivo);
    
    // Getters
    int getNumClientes() const;
    int getNumFuentesProd() const;
    double getCostoEnvio(int cliente, int fuenteProd) const;
    double getCostoActivacion(int fuenteProd) const;
    double getDemanda(int cliente) const;
    double getCapacidad(int fuenteProd) const;
    const vector<double>& getCapacidades() const;
    const vector<double>& getDemandas() const;
    
    // Utilidades
    void imprimir() const;
    bool esValido() const;
};

#endif // PROBLEMA_H
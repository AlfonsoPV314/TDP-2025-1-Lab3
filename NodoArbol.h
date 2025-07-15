// ============== NodoArbol.h ==============
#ifndef NODO_ARBOL_H
#define NODO_ARBOL_H

#include <vector>
#include <memory>
#include <map>

using namespace std;

class NodoArbol {
private:
    vector<shared_ptr<NodoArbol>> hijos;
    shared_ptr<NodoArbol> padre;
    double cotaInferior;
    double cotaSuperior;
    vector<double> solucionContinua;
    map<int, int> variablesFijadas;
    bool esFactible;
    int profundidad;

public:
    NodoArbol();
    NodoArbol(shared_ptr<NodoArbol> nodoPadre);

    void agregarHijo(shared_ptr<NodoArbol> hijo);
    void removerHijo(shared_ptr<NodoArbol> hijo);
    shared_ptr<NodoArbol> obtenerHijo(int indice) const;
    int obtenerNumHijos() const;
    
    double obtenerCotaInferior() const;
    void establecerCotaInferior(double cota);
    
    double obtenerCotaSuperior() const;
    void establecerCotaSuperior(double cota);
    
    const vector<double>& obtenerSolucion() const;
    void establecerSolucion(const vector<double>& sol);
    
    const map<int, int>& obtenerVariablesFijadas() const;
    void fijarVariable(int var, double valor);
    
    bool obtenerFactibilidad() const;
    void establecerFactibilidad(bool factible);
    
    int obtenerProfundidad() const;
    void establecerProfundidad(int prof);
    
    shared_ptr<NodoArbol> obtenerPadre() const;
    
    // Utilidades
    bool esHoja() const;
    bool esSolucionEntera() const;
    int obtenerVarMasFraccionaria() const;
    void imprimir() const;
};

#endif
all: testProblema testNodoArbol testResolveCFL testComparadorNodos testStrategyBranchAndBound main

Problema.o: Problema.cpp Problema.h
	@g++ -g -c Problema.cpp

NodoArbol.o: NodoArbol.cpp NodoArbol.h
	@g++ -g -c NodoArbol.cpp

ComparadorNodos.o: ComparadorNodos.cpp ComparadorNodos.h
	@g++ -g -c ComparadorNodos.cpp

StrategyBranchAndBound.o: StrategyBranchAndBound.cpp StrategyBranchAndBound.h
	@g++ -g -c StrategyBranchAndBound.cpp

ResolveCFL.o: ResolveCFL.cpp ResolveCFL.h
	@g++ -g -c ResolveCFL.cpp

testProblema: Problema.o testProblema.cpp
	@g++ -g Problema.o testProblema.cpp -o testProblema

testNodoArbol: NodoArbol.o testNodoArbol.cpp
	@g++ -g NodoArbol.o testNodoArbol.cpp -o testNodoArbol

testResolveCFL: Problema.o NodoArbol.o ComparadorNodos.o StrategyBranchAndBound.o ResolveCFL.o testResolveCFL.cpp
	@g++ -g Problema.o NodoArbol.o ComparadorNodos.o StrategyBranchAndBound.o ResolveCFL.o testResolveCFL.cpp \
		-lClp -lCoinUtils -lOsiClp -lOsi -o testResolveCFL

testComparadorNodos: NodoArbol.o ComparadorNodos.o testComparadorNodos.cpp
	@g++ -g NodoArbol.o ComparadorNodos.o testComparadorNodos.cpp -o testComparadorNodos

testStrategyBranchAndBound: Problema.o NodoArbol.o ComparadorNodos.o StrategyBranchAndBound.o testStrategyBranchAndBound.cpp
	@g++ -g Problema.o NodoArbol.o ComparadorNodos.o StrategyBranchAndBound.o testStrategyBranchAndBound.cpp \
		-lClp -lCoinUtils -lOsiClp -lOsi -o testStrategyBranchAndBound

main: Problema.o NodoArbol.o ComparadorNodos.o StrategyBranchAndBound.o ResolveCFL.o main.cpp
	@g++ -g Problema.o NodoArbol.o ComparadorNodos.o StrategyBranchAndBound.o ResolveCFL.o main.cpp \
		-lClp -lCoinUtils -lOsiClp -lOsi -o main

clean:
	@rm -rf *.o testProblema testNodoArbol testResolveCFL testComparadorNodos testStrategyBranchAndBound main
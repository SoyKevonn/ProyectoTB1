#pragma once
#include <functional>
using namespace std;
typedef unsigned int uint;

template <typename T, T NADA = 0>
class Lista
{
    struct Nodo;
    typedef function<int(T, T)> Comp;

    Nodo* ini;
    uint lon;
    Comp comparar;
public:
    Lista() :ini(nullptr), lon(0), comparar([](T a, T b) {return a - b; }) {}
    ~Lista();

    uint longitud();
    bool esVacia();

    void agregaInicial(T elem);
    void agregaPos(T elem, uint pos);
    void agregaFinal(T elem);

    void modificarInicial(T elem);
    void modificarPos(T elem, uint pos);
    void modificarFinal(T elem);

    void eliminaInicial();
    void eliminaPos(uint pos);
    void eliminaFinal();

    T obtenerInicial();
    T obtenerPos(uint pos);
    T obtenerFinal();

    T buscar(T elem);
};

template<typename T, T NADA>
struct Lista<T, NADA>::Nodo {
    T elem;
    Nodo* sig;
    Nodo(T elem = NADA, Nodo* sig = nullptr) : elem(elem), sig(sig) {}
};

template<typename T, T NADA>
Lista<T, NADA>::~Lista() {
    Nodo* aux = ini;
    while (aux != nullptr) {
        ini = ini->sig;
        delete aux;
        aux = ini;
    }
}

template<typename T, T NADA>
uint Lista<T, NADA>::longitud() {
    return lon;
}

template<typename T, T NADA>
bool Lista<T, NADA>::esVacia() {
    return lon == 0;
}

template<typename T, T NADA>
void Lista<T, NADA>::agregaInicial(T elem) {
    Nodo* nuevo = new Nodo(elem, ini);
    if (nuevo != nullptr) {
        ini = nuevo;
        lon++;
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::agregaPos(T elem, uint pos) {
    if (pos > lon) return;
    if (pos == 0) agregaInicial(elem);
    else {
        Nodo* aux = ini;
        for (uint i = 1; i < pos; i++) aux = aux->sig;
        Nodo* nuevo = new Nodo(elem, aux->sig);
        if (nuevo != nullptr) {
            aux->sig = nuevo;
            lon++;
        }
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::agregaFinal(T elem) {
    agregaPos(elem, lon);
}

template<typename T, T NADA>
void Lista<T, NADA>::modificarInicial(T elem) {
    if (lon > 0) ini->elem = elem;
}

template<typename T, T NADA>
void Lista<T, NADA>::modificarPos(T elem, uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo* aux = ini;
        for (uint i = 0; i < pos; i++) aux = aux->sig;
        aux->elem = elem;
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::modificarFinal(T elem) {
    modificarPos(elem, lon - 1);
}

template<typename T, T NADA>
void Lista<T, NADA>::eliminaInicial() {
    if (lon > 0) {
        Nodo* aux = ini;
        ini = ini->sig;
        delete aux;
        lon--;
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::eliminaPos(uint pos) {
    if (pos == 0) { eliminaInicial(); return; }
    if (pos < lon) {
        Nodo* aux = ini;
        for (uint i = 1; i < pos; i++) aux = aux->sig;
        Nodo* aBorrar = aux->sig;
        aux->sig = aBorrar->sig;
        delete aBorrar;
        lon--;
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::eliminaFinal() {
    if (lon > 0) eliminaPos(lon - 1);
}

template<typename T, T NADA>
T Lista<T, NADA>::obtenerInicial() {
    return obtenerPos(0);
}

template<typename T, T NADA>
T Lista<T, NADA>::obtenerPos(uint pos) {
    if (pos >= 0 && pos < lon) {
        Nodo* aux = ini;
        for (uint i = 0; i < pos; i++) aux = aux->sig;
        return aux->elem;
    }
    else return NADA;
}

template<typename T, T NADA>
T Lista<T, NADA>::obtenerFinal() {
    return obtenerPos(lon - 1);
}

template<typename T, T NADA>
T Lista<T, NADA>::buscar(T elem) {
    Nodo* aux = ini;
    while (aux != nullptr) {
        if (comparar(aux->elem, elem) == 0)
            return aux->elem;
        aux = aux->sig;
    }
    return NADA;
}

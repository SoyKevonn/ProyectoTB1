#pragma once
#ifndef COLA_H
#define COLA_H


template <typename T>
struct NodoCola {
    T dato;
    NodoCola* siguiente;
    NodoCola(T v) : dato(v), siguiente(nullptr) {}
};

template <typename T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* final;

public:
    Cola() : frente(nullptr), final(nullptr) {}

    void encolar(T valor) {
        NodoCola<T>* nuevo = new NodoCola<T>(valor);
        if (!frente) {
            frente = final = nuevo;
        }
        else {
            final->siguiente = nuevo;
            final = nuevo;
        }
    }

    bool estaVacia() { return frente == nullptr; }
  
};

#endif
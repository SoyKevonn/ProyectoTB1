#pragma once
#include "Nodo.h"

using namespace std;

template<typename T>
class Pila {

private:
	Nodo<T>* cima;

public:

	Pila() {

		cima = nullptr;
	}

	bool isEmpty() {

		return cima == nullptr;

	}

	void push(T valor);
	T pop();




};

template<typename T>
void Pila<T>::push(T valor) {
	if (isEmpty()) {
		cima = new Nodo<T>(valor);
	}
	else {
		cima = new Nodo<T>(valor, cima);
	}

}

template<typename T>
T Pila<T>::pop() {

	if (isEmpty()) return T();
	else {
		Nodo<T>* elim = cima;
		T elemento = cima->dato;
		cima = cima->siguiente;
		delete elim;
		return elemento;
	}

}
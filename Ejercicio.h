#pragma once
#include "Pila.h"
#include <vector>


template<typename T>
class Ejercicio {
private:
	int idEjercicio;
	Pila<T> preguntaspendientes;

public:
	int totalPreguntas;
	int acertadas;

	Ejercicio(vector<T> preguntas) : acertadas(0) {
		static int contadorId = 0;
		idEjercicio = contadorId++;

		vector<T> faciles, medios, dificiles;
		for (T& p : preguntas) {
			if (p.getDificultad() == 1) faciles.push_back(p);
			else if (p.getDificultad() == 2) medios.push_back(p);
			else dificiles.push_back(p);
		}

		//Lambda con un Fisher-Yates Shuffle para mezclar las preguntas

		auto shuffleVector = [](vector<T>& vec) {
			int n = (int)vec.size();
			for (int i = n - 1; i > 0; --i) {
				int j = rand() % (i + 1);
				swap(vec[i], vec[j]);
			}
			};

		shuffleVector(faciles);
		shuffleVector(medios);
		shuffleVector(dificiles);

		totalPreguntas = 0;

		// 1. Insertamos 1 Difícil (quedará al fondo de la pila)
		for (int k = 0; k < 1 && k < dificiles.size(); ++k) {
			preguntaspendientes.push(dificiles[k]);
			totalPreguntas++;
		}

		// 2. Insertamos  2 Medias
		for (int k = 0; k < 2 && k < medios.size(); ++k) {
			preguntaspendientes.push(medios[k]);
			totalPreguntas++;
		}

		// 3. Insertamos 3 Fáciles (quedarán en la cima, serán las primeras en salir)
		for (int k = 0; k < 3 && k < faciles.size(); ++k) {
			preguntaspendientes.push(faciles[k]);
			totalPreguntas++;
		}
	}


	T obtenerSiguientePregunta();
	void registarRespuesta(bool respuestaCorrecta);
	void reinsertarPregunta(T p);
	bool Terminado();


};

	template<typename T>
	T Ejercicio<T>::obtenerSiguientePregunta() {
		if (preguntaspendientes.isEmpty()) return T();
		return preguntaspendientes.pop();
	}

	template<typename T>
	void Ejercicio<T>::registarRespuesta(bool respuestaCorrecta) {
		if (respuestaCorrecta) acertadas++;
		if (!respuestaCorrecta && acertadas >= 0) acertadas --;
	}

	template<typename T>
	void Ejercicio<T>::reinsertarPregunta(T p) {
		preguntaspendientes.push(p);
	}

	template<typename T>
	bool Ejercicio<T>::Terminado() {
		return preguntaspendientes.isEmpty();
	}
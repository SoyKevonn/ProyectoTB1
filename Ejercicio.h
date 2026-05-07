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

	// usuarioSeed: ID del usuario — garantiza que cada usuario reciba preguntas distintas
	// intentoUsuario: cuantas veces ha jugado esta leccion — rota las preguntas en cada reintento
	Ejercicio(vector<T> preguntas, int usuarioSeed = 0, int intentoUsuario = 0) : acertadas(0) {
		static int contadorId = 0;
		idEjercicio = contadorId++;

		vector<T> faciles, medios, dificiles;
		for (T& p : preguntas) {
			if (p.getDificultad() == 1) faciles.push_back(p);
			else if (p.getDificultad() == 2) medios.push_back(p);
			else dificiles.push_back(p);
		}

		// Fisher-Yates Shuffle con seed unica por usuario e intento
		// Kevin y Alfredo ven preguntas distintas; cada reintento tambien rota el orden
		auto shuffleVector = [&](vector<T>& vec) {
			int n = (int)vec.size();
			// combinar ID de usuario + intento + idEjercicio como seed
			unsigned int seed = (unsigned int)(usuarioSeed * 1000 + intentoUsuario * 100 + idEjercicio);
			srand(seed);
			for (int i = n - 1; i > 0; --i) {
				int j = rand() % (i + 1);
				swap(vec[i], vec[j]);
			}
			};

		shuffleVector(faciles);
		shuffleVector(medios);
		shuffleVector(dificiles);

		totalPreguntas = 0;

		// Tomar hasta 2 dificiles, 3 medias, 4 faciles — aprovecha mejor el banco amplio
		// se insertan en orden inverso: dificiles al fondo de la pila, faciles en la cima (salen primero)
		for (int k = 0; k < 2 && k < (int)dificiles.size(); ++k) {
			preguntaspendientes.push(dificiles[k]);
			totalPreguntas++;
		}
		for (int k = 0; k < 3 && k < (int)medios.size(); ++k) {
			preguntaspendientes.push(medios[k]);
			totalPreguntas++;
		}
		for (int k = 0; k < 4 && k < (int)faciles.size(); ++k) {
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
	if (!respuestaCorrecta && acertadas >= 0) acertadas--;
}

template<typename T>
void Ejercicio<T>::reinsertarPregunta(T p) {
	preguntaspendientes.push(p);
}

template<typename T>
bool Ejercicio<T>::Terminado() {
	return preguntaspendientes.isEmpty();
}
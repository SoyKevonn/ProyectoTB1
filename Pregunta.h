#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Pregunta {
private:
	int idEjercicio;
	string Enunciado;
	vector<string> opciones;
	string respuestaCorrecta;
	int dificultad; // 1: Facil, 2: Medio, 3: Dificil
	bool esCorrecta;

public:
	Pregunta() : idEjercicio(-1), Enunciado(""), respuestaCorrecta(""), dificultad(1), esCorrecta(false) {}
	Pregunta(string en, vector<string> op, string rc, int dif) : Enunciado(en), opciones(op), respuestaCorrecta(rc), dificultad(dif), esCorrecta(false) {
		static int contadorId = 0;
		idEjercicio = contadorId++;
	}

	bool verificarRespuesta(string respuesta) {
		esCorrecta = (respuesta == respuestaCorrecta);
		return esCorrecta;
	}

	string getEnunciado() { return Enunciado; }
	vector<string> getOpciones() { return opciones; }
	string getRespuestaCorrecta() { return respuestaCorrecta; }
	int getDificultad() { return dificultad; }
	bool getEsCorrecta() { return esCorrecta; }

};
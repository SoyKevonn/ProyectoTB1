#pragma once
#include <iostream>
#include <vector>    
#include <fstream>   
#include <sstream>   
#include <string>
#include "Pregunta.h"

using namespace std;

class Leccion {
private:
    string tema;
    	string rutaArchivo;
    vector<Pregunta> bancoDePreguntas;
    bool desbloqueada;

    int acertadas;
    int ultimototal;
    bool hajugado;

public:
    Leccion() : tema("Sin tema"), rutaArchivo(""), desbloqueada(false), acertadas(0), ultimototal(0), hajugado(false) {}
    Leccion(string t, string r, bool des) : tema(t), rutaArchivo(r), desbloqueada(des), acertadas(0), ultimototal(0), hajugado(false) {}


	void desbloquear() { desbloqueada = true; }
	bool estaDesbloqueada() { return desbloqueada; }
    void setDesbloqueada(bool d) { desbloqueada = d; }

    string getTema() { return tema; }

	string getRutaArchivo() { return rutaArchivo; }

    vector<Pregunta> getPreguntas() { return bancoDePreguntas; }

   
    bool cargarArchivo();

    void guardarResultados(int acertadas, int total);
	void jugado() { hajugado = true; }
    double obtenerpromedio() {
        if (ultimototal == 0) return 0.0;
        return (double)acertadas / ultimototal * 100.0;
	}
};

bool Leccion::cargarArchivo() {
    if (rutaArchivo == "") return false;
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) return false;

    bancoDePreguntas.clear();

    getline(archivo, tema);

    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string en, op1, op2, op3, rc, dif_str;

        getline(ss, en, '|');
        getline(ss, op1, '|');
        getline(ss, op2, '|');
        getline(ss, op3, '|');
        getline(ss, rc, '|');
        getline(ss, dif_str, '|');

        int dificultad = stoi(dif_str);
        vector<string> opciones = { op1, op2, op3 };
        Pregunta nuevaPregunta(en, opciones, rc, dificultad);
        bancoDePreguntas.push_back(nuevaPregunta);
    }

    archivo.close();
    return true;

}

void Leccion::guardarResultados(int a, int total) {
    acertadas = a;
    ultimototal = total;
    hajugado = true;
}


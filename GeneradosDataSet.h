#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

class GeneradorDataSet {
private:
    
    int obtenerAleatorio(int min, int max) {
        return min + (rand() % (max - min + 1));
    }

public:
    GeneradorDataSet() {
        // Inicializar la semilla
        srand((unsigned)time(NULL));
    }

    // Estructura
    struct ParVocabulario {
        string espanol;
        string ingles;
        int dificultadBase;
    };

    // Genera un archivo .txt con formato compatible para LinguaTrack
    void generarLeccionVocabulario(string rutaSalida, string tema, int sourceId, vector<ParVocabulario>& baseDatos, int cantidadPreguntas) {
        ofstream archivo(rutaSalida);

        if (!archivo.is_open()) {
            cout << "  Error: No se pudo crear el archivo " << rutaSalida << endl;
            return;
        }

        archivo << "" << tema << "\n";

        int totalPalabras = baseDatos.size();

        // Evitar bucles infinitos si pedimos más preguntas que palabras disponibles
        if (cantidadPreguntas > totalPalabras) cantidadPreguntas = totalPalabras;

        // Mezclar la base de datos para no tener siempre las mismas preguntas
        random_shuffle(baseDatos.begin(), baseDatos.end());

        for (int i = 0; i < cantidadPreguntas; i++) {
            ParVocabulario actual = baseDatos[i];

            bool esInglesAEspanol = (rand() % 2 == 0);
            string enunciado, respuestaCorrecta;

            if (esInglesAEspanol) {
                enunciado = "Que significa '" + actual.ingles + "'?";
                respuestaCorrecta = actual.espanol;
            }
            else {
                enunciado = "Como se dice '" + actual.espanol + "'?";
                respuestaCorrecta = actual.ingles;
            }

            vector<string> opciones;
            opciones.push_back(respuestaCorrecta);

            while (opciones.size() < 3) {
                int indiceAzar = obtenerAleatorio(0, totalPalabras - 1);
                string distractor = esInglesAEspanol ? baseDatos[indiceAzar].espanol : baseDatos[indiceAzar].ingles;

                // Asegurarse de no repetir la respuesta correcta ni agregar un distractor duplicado
                if (distractor != respuestaCorrecta && find(opciones.begin(), opciones.end(), distractor) == opciones.end()) {
                    opciones.push_back(distractor);
                }
            }

            // Mezclar las 3 opciones para que la correcta cambie de posición
            random_shuffle(opciones.begin(), opciones.end());

            // Escribir en el archivo con el formato: Enunciado|op1|op2|op3|Correcta|Dificultad
            archivo << enunciado << "|"
                << opciones[0] << "|"
                << opciones[1] << "|"
                << opciones[2] << "|"
                << respuestaCorrecta << "|"
                << actual.dificultadBase << "\n";
        }

        archivo.close();
        cout << "  DataSet generado exitosamente: " << rutaSalida << " con " << cantidadPreguntas << " preguntas." << endl;
    }
};
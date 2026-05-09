#pragma once
#include "Fondo.h"
#include "Lista.h"
#include "Leccion.h"
#include "Ejercicio.h"
#include "Usuario.h"
#include "GestionUsuario.h"
#include <cstdlib>
#include <conio.h>


using namespace std;
using namespace System;


class GestionLecciones {
private:
    Lista<Leccion*> misLecciones;
    Usuario<int>* usuarioActivo = nullptr;
public:
    GestionLecciones() {
        misLecciones.agregaFinal(new Leccion("Verbos", "verbos.txt", true));
        misLecciones.agregaFinal(new Leccion("Animales", "animales.txt", false));
        misLecciones.agregaFinal(new Leccion("Colores", "colores.txt", false));
        misLecciones.agregaFinal(new Leccion("Familia", "familia.txt", false));
        misLecciones.agregaFinal(new Leccion("Comidas", "comidas.txt", false));
    }

    // funciones que podrian ayudar a alfredo
    Leccion* getLeccion(uint indice) {
        return misLecciones.obtenerPos(indice);

    }


    uint cantidadLecciones() {
        return misLecciones.longitud();
    }

    void mostrarMenu() {
        cout << "  Ingresa tu ID de usuario: ";
        int idU; cin >> idU; cin.ignore();
        usuarioActivo = buscarUsuarioPorId(idU);

        if (usuarioActivo == nullptr) {
            cout << "  Usuario no encontrado." << endl;
            return;
        }

        cout << "  Bienvenido, " << usuarioActivo->getNombre() << "!" << endl;

        _sleep(2000);
        limpiarPantalla();

        // resetear todas las lecciones a bloqueado primero
        for (uint i = 0; i < misLecciones.longitud(); i++) {
            misLecciones.obtenerPos(i)->setDesbloqueada(false);
        }

        // la primera siempre esta disponible
        misLecciones.obtenerPos(0)->desbloquear();

        // desbloquear las que el usuario ya completo
        int completadas = usuarioActivo->getLeccionesCompletadas();
        for (int i = 1; i <= completadas && i < (int)misLecciones.longitud(); i++) {
            misLecciones.obtenerPos(i)->desbloquear();
        }

        char tecla;
        int opcion;

        do {
			dibujarFondoLeccionMenu();

            int x = 25;
            int y = 13;

            Console::ForegroundColor = ConsoleColor::Green;

            for (uint i = 0; i < misLecciones.longitud(); i++) {
                Leccion* l = misLecciones.obtenerPos(i);

                string linea = to_string(i + 1) + ". " + l->getTema();
                if (!l->estaDesbloqueada()) {
                    linea += " [BLOQUEADA]";
                }

                escribirFondoLeccionMenu(linea, x, y + i + 1, 5);
            }

            escribirFondoLeccionMenu("0. Salir\n", 25, 20, 2);

            Console::ResetColor();

            tecla = _getch();
            opcion = tecla - '0';

            limpiarPantalla();

            if (opcion > 0 && opcion <= (int)misLecciones.longitud()) {
                uint indice = opcion - 1;
                Leccion* seleccionada = misLecciones.obtenerPos(indice);

                if (seleccionada->estaDesbloqueada()) {
                    ejecutarLeccion(indice);
                }
                else {
                    cout << "\n  Debes completar la leccion anterior primero!\n";
                    _sleep(1000);
                }
            }

        } while (opcion != 0);

        // resetear al salir para limpiar para el proximo usuario
        for (uint i = 1; i < misLecciones.longitud(); i++) {
            misLecciones.obtenerPos(i)->setDesbloqueada(false);
        }
    }
    
    void ejecutarLeccion(uint indice) {

        //Lambda para calcular el promedio de aciertos
        auto calcularPromedio = [](int aciertos, int total){
			if (total == 0) return 0.0;
			return (double)aciertos / total * 100.0;
            };

        Leccion* l = misLecciones.obtenerPos(indice);
        if (!l->cargarArchivo()) return;

       // seed = ID del usuario; intento = lecciones completadas (cambia cada vez que aprueba)
        int intentoLeccion = usuarioActivo->getLeccionesCompletadas();
        Ejercicio<Pregunta> sesion(l->getPreguntas(), usuarioActivo->getId(), intentoLeccion);

        limpiarPantalla();
		dibujarFondoLeccionPregunta(9);
        escribirFondoLeccionMenu("\n Iniciando: " + l->getTema(), 23, 16, 5);
        escribirFondoLeccionMenu("  Total de preguntas: " + to_string(sesion.totalPreguntas), 23, 17, 5);
        escribirFondoLeccionMenu("  Necesitas responder mas de la mitad para desbloquear la siguiente", 23, 18, 5);
		escribirFondoLeccionMenu("  leccion.", 23, 19, 5);
        escribirFondoLeccionMenu("", 23, 19, 5);

        getch();
        limpiarPantalla();

        while (!sesion.Terminado()) {

            char opcionRespuesta;
            Pregunta p = sesion.obtenerSiguientePregunta();
            int x2 = 25; int y2 = 18;

			dibujarFondoLeccionPregunta(9);

            string cabecera = "[Dificultad: " + to_string(p.getDificultad()) + "] " + p.getEnunciado();
            escribirFondoLeccionMenu(cabecera, 25, 16, 5);


            for (int i = 0; i < (int)p.getOpciones().size(); i++) {

                string letra(1, char('A' + i));
                string opcion = letra + ". " + p.getOpciones()[i];

                escribirFondoLeccionMenu(opcion, x2 + 4, y2 + 1 + i, 5);
            }
            

            do {
                opcionRespuesta = toupper(_getch()); // Convertimos a mayúscula de una vez
            } while (opcionRespuesta != 'A' && opcionRespuesta != 'B' && opcionRespuesta != 'C');


            string respuestaElegida = "";
            if (opcionRespuesta == 'A' || opcionRespuesta == 'a') respuestaElegida = p.getOpciones()[0];
            else if (opcionRespuesta == 'B' || opcionRespuesta == 'b') respuestaElegida = p.getOpciones()[1];
            else if (opcionRespuesta == 'C' || opcionRespuesta == 'c') respuestaElegida = p.getOpciones()[2];


            p.verificarRespuesta(respuestaElegida);

            if (p.getEsCorrecta()) {
                dibujarFondoLeccionPregunta(1);
                sesion.registarRespuesta(true);
				escribirFondoLeccionMenu("  Correcto!", 25, 20, 5);
            }
            else {
                dibujarFondoLeccionPregunta(0);
                sesion.registarRespuesta(false);
                sesion.reinsertarPregunta(p);

				string incorrecto = "  Incorrecto. La respuesta correcta era: " + p.getRespuestaCorrecta();
                escribirFondoLeccionMenu(incorrecto, 25, 20, 2);
            }

            getch();
            limpiarPantalla();
        }

        // mostrar resultado final
        limpiarPantalla();
        dibujarFondoLeccionPregunta(3);
		escribirFondoLeccionMenu("\n == = RESULTADO == = ", 25, 16, 5);

		string leccionTema = "  Leccion  :" + l->getTema();
        escribirFondoLeccionMenu(leccionTema, 25, 18, 5);

        string aciertos = "  Aciertos : " + to_string(sesion.acertadas) + " de " + to_string(sesion.totalPreguntas);
        escribirFondoLeccionMenu(aciertos, 25, 19, 5);

        bool aprobo = sesion.acertadas >= (sesion.totalPreguntas / 2);

		string porcetajeprom = "  Promedio  : " + to_string(calcularPromedio(sesion.acertadas, sesion.totalPreguntas)) + "%";
        escribirFondoLeccionMenu(porcetajeprom, 25, 20, 5);

        if (aprobo) {
            // actualizar lecciones completadas solo si es nueva para el usuario
            int actuales = usuarioActivo->getLeccionesCompletadas();
            if ((int)indice >= actuales) {
                usuarioActivo->setLeccionesCompletadas(actuales + 1);
            }

            // desbloquear la siguiente leccion en memoria
            if (indice + 1 < misLecciones.longitud()) {
                Leccion* proxima = misLecciones.obtenerPos(indice + 1);
                if (!proxima->estaDesbloqueada()) {
                    proxima->desbloquear();
                    escribirFondoLeccionMenu("  Has desbloqueado: " + proxima->getTema() + "!", 25, 22, 5);
                }
            }

            // dar puntos al usuario
            int puntos = sesion.acertadas * 20;
            if (puntos > 0) {
                usuarioActivo->agregarPuntos(puntos);
                escribirFondoLeccionMenu("  +" + to_string(puntos) + " puntos agregados a tu cuenta!", 25, 23, 5);
                usuarioActivo->enviarNotificacion(
                    "Completaste: " + l->getTema() + " con " + to_string(sesion.acertadas) + " aciertos",
                    "Hoy"
                );
            }

            l->guardarResultados(sesion.acertadas, sesion.totalPreguntas);
        }
        else {
			escribirFondoLeccionMenu("  No alcanzaste el minimo para aprobar esta leccion.", 25, 21, 2);
			escribirFondoLeccionMenu("  Revisa tus respuestas y vuelve a intentarlo.", 25, 22, 2);
        }

        // persistir cambios del usuario en archivo
        reescribirArchivoUsuarios();

        cout << endl;
        getch();
        limpiarPantalla();
    }
};

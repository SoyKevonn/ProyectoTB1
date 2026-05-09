#pragma once
#include "Lista.h"
#include "Leccion.h"
#include "Ejercicio.h"
#include "Usuario.h"
#include "GestionUsuario.h"
#include <cstdlib>
#include <conio.h>


using namespace System;
using namespace std;


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
            // Metodo a cambiar por otro nuevo DibujarMapaM();
            Console::ForegroundColor = ConsoleColor::White;
            // Metodo a cambiar por otro nuevo escribirEnMapaM("       === LECCIONES ===       ", 0, 1, 2);
            
            int x = 0;
            int y = 3;

            Console::ForegroundColor = ConsoleColor::Green;

            for (uint i = 0; i < misLecciones.longitud(); i++) {
                Leccion* l = misLecciones.obtenerPos(i);

                string linea = to_string(i + 1) + ". " + l->getTema();
                if (!l->estaDesbloqueada()) {
                    linea += " [BLOQUEADA]";
                }

                //escribirEnMapaM(linea, x, y + i + 1, 3);
            }

            Console::ForegroundColor = ConsoleColor::Red;
            //escribirEnMapaM("0. Salir\n", 0, 9, 4);

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
        Leccion* l = misLecciones.obtenerPos(indice);
        if (!l->cargarArchivo()) return;

       // seed = ID del usuario; intento = lecciones completadas (cambia cada vez que aprueba)
        int intentoLeccion = usuarioActivo->getLeccionesCompletadas();
        Ejercicio<Pregunta> sesion(l->getPreguntas(), usuarioActivo->getId(), intentoLeccion);

        limpiarPantalla();
        cout << "\n  Iniciando: " << l->getTema() << endl;
        cout << "  Total de preguntas: " << sesion.totalPreguntas << endl;
        cout << "  Necesitas responder mas de la mitad para desbloquear la siguiente leccion." << endl;
        cout << endl;
        getch();
        limpiarPantalla();

        while (!sesion.Terminado()) {
            char opcionRespuesta;
            Pregunta p = sesion.obtenerSiguientePregunta();

            cout << "  [Dificultad: " << p.getDificultad() << "] " << p.getEnunciado() << endl << endl;

            for (int i = 0; i < (int)p.getOpciones().size(); i++) {
                cout << "    " << char('A' + i) << ". " << p.getOpciones()[i] << endl;
            }

            cout << "\n  Elige (A, B o C): ";
            opcionRespuesta = _getch();

            string respuestaElegida = "";
            if (opcionRespuesta == 'A' || opcionRespuesta == 'a') respuestaElegida = p.getOpciones()[0];
            else if (opcionRespuesta == 'B' || opcionRespuesta == 'b') respuestaElegida = p.getOpciones()[1];
            else if (opcionRespuesta == 'C' || opcionRespuesta == 'c') respuestaElegida = p.getOpciones()[2];

            p.verificarRespuesta(respuestaElegida);

            if (p.getEsCorrecta()) {
                sesion.registarRespuesta(true);
                cout << "\n  Correcto!" << endl;
            }
            else {
                sesion.registarRespuesta(false);
                sesion.reinsertarPregunta(p);
                cout << "\n  Incorrecto. La respuesta correcta era: " << p.getRespuestaCorrecta() << endl;
            }

            getch();
            limpiarPantalla();
        }

        // mostrar resultado final
        limpiarPantalla();
        cout << "\n  === RESULTADO ===" << endl;
        cout << "  Leccion  : " << l->getTema() << endl;
        cout << "  Aciertos : " << sesion.acertadas << " de " << sesion.totalPreguntas << endl;

        bool aprobo = sesion.acertadas >= (sesion.totalPreguntas / 2);

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
                    cout << "  Has desbloqueado: " << proxima->getTema() << "!" << endl;
                }
            }

            // dar puntos al usuario
            int puntos = sesion.acertadas * 20;
            if (puntos > 0) {
                usuarioActivo->agregarPuntos(puntos);
                cout << "  +" << puntos << " puntos agregados a tu cuenta!" << endl;
                usuarioActivo->enviarNotificacion(
                    "Completaste: " + l->getTema() + " con " + to_string(sesion.acertadas) + " aciertos",
                    "Hoy"
                );
            }

            l->guardarResultados(sesion.acertadas, sesion.totalPreguntas);
        }
        else {
            cout << "  No alcanzaste el minimo para desbloquear la siguiente leccion." << endl;
            cout << "  Intentalo de nuevo." << endl;
        }

        // persistir cambios del usuario en archivo
        reescribirArchivoUsuarios();

        cout << endl;
        getch();
        limpiarPantalla();
    }
};

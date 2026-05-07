#pragma once
#include "Lista.h"
#include "Leccion.h"
#include "Ejercicio.h"
#include "Usuario.h"
#include "GestionUsuario.h"
#include <cstdlib>
#include <conio.h>



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

        int opcion;
        do {
            cout << "\n=== MENU DE LECCIONES ===\n";
            for (uint i = 0; i < misLecciones.longitud(); i++) {
                Leccion* l = misLecciones.obtenerPos(i);
                cout << i + 1 << ". " << l->getTema();
                if (!l->estaDesbloqueada()) cout << " [BLOQUEADA]";
                cout << endl;
            }
            cout << "0. Salir\n";
            cout << "Selecciona una leccion: ";
            cin >> opcion;
            cin.ignore();
            limpiarPantalla();

            if (opcion > 0 && opcion <= (int)misLecciones.longitud()) {
                uint indice = opcion - 1;
                Leccion* seleccionada = misLecciones.obtenerPos(indice);

                if (seleccionada->estaDesbloqueada()) {
                    ejecutarLeccion(indice);
                }
                else {
                    cout << "\n  Debes completar la leccion anterior primero!\n";
                    pausar();
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

        Ejercicio<Pregunta> sesion(l->getPreguntas());

        limpiarPantalla();
        cout << "\n  Iniciando: " << l->getTema() << endl;
        cout << "  Total de preguntas: " << sesion.totalPreguntas << endl;
        cout << "  Necesitas responder mas de la mitad para desbloquear la siguiente leccion." << endl;
        cout << endl;
        getch();
        limpiarPantalla();

        while (!sesion.Terminado()) {
            string opcionRespuesta;
            Pregunta p = sesion.obtenerSiguientePregunta();

            cout << "  [Dificultad: " << p.getDificultad() << "] " << p.getEnunciado() << endl << endl;

            for (int i = 0; i < (int)p.getOpciones().size(); i++) {
                cout << "    " << char('A' + i) << ". " << p.getOpciones()[i] << endl;
            }

            cout << "\n  Elige (A, B o C): ";
            cin >> opcionRespuesta;
            cin.ignore();

            string respuestaElegida = "";
            if (opcionRespuesta == "A" || opcionRespuesta == "a") respuestaElegida = p.getOpciones()[0];
            else if (opcionRespuesta == "B" || opcionRespuesta == "b") respuestaElegida = p.getOpciones()[1];
            else if (opcionRespuesta == "C" || opcionRespuesta == "c") respuestaElegida = p.getOpciones()[2];

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
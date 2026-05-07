#pragma once
#ifndef GESTIONPROGRESO_H
#define GESTIONPROGRESO_H

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "GestionUsuario.h"     // listaUsuarios, buscarUsuarioPorId
#include "GestionLecciones.h"   // GestionLecciones*
#include "NivelInfo.h"
#include "EstadisticaUsuario.h"

using namespace std;

// =========================================================
// METODO RECURSIVO (de Alfredo, conservado)
// Calcula cuantas sesiones faltan para llegar a una meta
// dado un promedio de puntos por sesion
// =========================================================
int sesionesParaObjetivo(int puntosFaltantes, int promedioPorSesion) {
    if (puntosFaltantes <= 0) return 0;
    return 1 + sesionesParaObjetivo(puntosFaltantes - promedioPorSesion, promedioPorSesion);
}

// =========================================================
// ALGORITMO DE ORDENAMIENTO (de Alfredo, conservado)
// Insertion sort descendente para ranking de puntajes
// =========================================================
void rankingGeneral(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int clave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < clave) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = clave;
    }
}

// =========================================================
// MENU PRINCIPAL DE PROGRESO
// Recibe puntero al GestionLecciones ya instanciado en main
// =========================================================
void menuProgreso(GestionLecciones* gL) {

    // --- Pedir ID y buscar usuario ---
    limpiarPantalla();
    cout << "========================================" << endl;
    cout << "          MODULO DE PROGRESO            " << endl;
    cout << "========================================" << endl;
    cout << "  Ingresa tu ID de usuario: ";
    int idU; cin >> idU; cin.ignore();

    Usuario<int>* u = buscarUsuarioPorId(idU);
    if (u == nullptr) {
        cout << "\n  Usuario no encontrado." << endl;
        pausar();
        return;
    }

    // Helpers que usan datos reales del usuario
    NivelInfo<int>        nivelInfo;
    EstadisticaUsuario<int> stats;

    // --- LAMBDAS ---

    // Barra de progreso basada en puntos reales del usuario
    auto generarBarra = [](int p) {
        int tope = 1000;
        int barras = (p >= tope) ? 10 : (p * 10 / tope);
        cout << "  [";
        for (int i = 0; i < barras; i++)       cout << "#";
        for (int i = barras; i < 10; i++)      cout << "-";
        cout << "] " << p << " / " << tope << " pts" << endl;
        };

    // Calcula puntos faltantes para una meta
    auto puntosFaltantes = [](int meta, int actual) -> int {
        return (meta > actual) ? meta - actual : 0;
        };

    int opcion;
    do {
        limpiarPantalla();
        cout << "========================================" << endl;
        cout << "          MODULO DE PROGRESO            " << endl;
        cout << "  Usuario: " << u->getNombre() << " " << u->getApellido()
            << " [" << u->getSuscripcion()->getTipo() << "]" << endl;
        cout << "========================================" << endl;
        cout << "  1. Ver mi progreso" << endl;
        cout << "  2. Ver lecciones" << endl;
        cout << "  3. Estadisticas" << endl;
        cout << "  4. Ranking de estudiantes" << endl;
        cout << "  5. Proyectar meta" << endl;
        cout << "  0. Volver al menu principal" << endl;
        cout << "\n  Opcion: ";
        cin >> opcion; cin.ignore();

        // --------------------------------------------------
        switch (opcion) {

            // ---- 1. PROGRESO GENERAL ----
        case 1: {
            limpiarPantalla();
            cout << "\n  === PROGRESO DE " << u->getNombre() << " ===" << endl << endl;

            cout << "  Puntos totales : " << u->getPuntos() << endl;
            cout << "  Nivel          : " << u->getNivel()
                << " (" << nivelInfo.obtenerRango(u->getPuntos()) << ")" << endl;
            cout << "  Racha actual   : " << u->getPerfil()->getRacha() << " dias" << endl;
            cout << "  Lecciones completadas: " << u->getLeccionesCompletadas()
                << " de " << gL->cantidadLecciones() << endl << endl;

            cout << "  Barra de progreso (meta: 1000 pts):" << endl;
            cout << "  ";
            generarBarra(u->getPuntos());

            cout << endl;
            pausar();
            break;
        }

              // ---- 2. ESTADO DE LECCIONES ----
        case 2: {
            limpiarPantalla();
            cout << "\n  === LECCIONES ===" << endl << endl;

            // Reconstruir estado de desbloqueo igual que GestionLecciones::mostrarMenu
            // sin modificar el estado real — solo lectura
            int completadas = u->getLeccionesCompletadas();

            for (uint i = 0; i < gL->cantidadLecciones(); i++) {
                Leccion* l = gL->getLeccion(i);
                bool desbloqueada = (i == 0 || (int)i <= completadas);
                bool terminada = ((int)i < completadas);

                cout << "  " << i + 1 << ". " << l->getTema();
                if (terminada)         cout << "  [COMPLETADA]";
                else if (desbloqueada) cout << "  [DISPONIBLE]";
                else                   cout << "  [BLOQUEADA]";
                cout << endl;
            }

            cout << endl;
            pausar();
            break;
        }

              // ---- 3. ESTADISTICAS ----
        case 3: {
            limpiarPantalla();
            cout << "\n  === ESTADISTICAS ===" << endl << endl;

            // Construir arreglo de puntos por leccion completada
            // Estimamos puntos promedio por leccion segun total / completadas
            int completadas = u->getLeccionesCompletadas();
            if (completadas == 0) {
                cout << "  Aun no has completado ninguna leccion." << endl;
            }
            else {
                // Distribucion uniforme de puntos como aproximacion
                vector<int> puntosLeccion;
                int base = u->getPuntos() / completadas;
                for (int i = 0; i < completadas; i++)
                    puntosLeccion.push_back(base);

                int promedio = stats.calcularPromedio(puntosLeccion.data(), completadas);

                cout << "  Lecciones completadas : " << completadas << endl;
                cout << "  Puntos totales        : " << u->getPuntos() << endl;
                cout << "  Promedio por leccion  : " << promedio << " pts" << endl;
                cout << "  Rango actual          : " << nivelInfo.obtenerRango(u->getPuntos()) << endl;
            }

            cout << endl;
            pausar();
            break;
        }

              // ---- 4. RANKING DE ESTUDIANTES ----
        case 4: {
            limpiarPantalla();
            cout << "\n  === RANKING DE ESTUDIANTES ===" << endl << endl;

            int n = (int)listaUsuarios.longitud();
            if (n == 0) {
                cout << "  No hay usuarios registrados." << endl;
                pausar();
                break;
            }

            // Extraer puntajes y nombres para ordenar
            vector<pair<int, string>> tabla;
            for (uint i = 0; i < listaUsuarios.longitud(); i++) {
                Usuario<int>* usr = listaUsuarios.obtenerPos(i);
                if (usr) tabla.push_back({ usr->getPuntos(),
                    usr->getNombre() + " " + usr->getApellido() });
            }

            // Insertion sort descendente (algoritmo de Alfredo adaptado a vector de pares)
            for (int i = 1; i < (int)tabla.size(); i++) {
                auto clave = tabla[i];
                int j = i - 1;
                while (j >= 0 && tabla[j].first < clave.first) {
                    tabla[j + 1] = tabla[j];
                    j--;
                }
                tabla[j + 1] = clave;
            }

            for (int i = 0; i < (int)tabla.size(); i++) {
                cout << "  " << i + 1 << ". " << tabla[i].second
                    << " - " << tabla[i].first << " pts";
                if (tabla[i].second == u->getNombre() + " " + u->getApellido())
                    cout << "  <- tu";
                cout << endl;
            }

            cout << endl;
            pausar();
            break;
        }

              // ---- 5. PROYECTAR META ----
        case 5: {
            limpiarPantalla();
            cout << "\n  === PROYECTAR META ===" << endl << endl;

            int meta;
            cout << "  Puntos actuales : " << u->getPuntos() << endl;
            cout << "  Ingresa tu meta de puntos: "; cin >> meta; cin.ignore();

            int faltan = puntosFaltantes(meta, u->getPuntos());
            int necesarias = sesionesParaObjetivo(faltan, 25);

            if (faltan == 0) {
                cout << "\n  Ya alcanzaste esa meta! Elige un numero mayor." << endl;
            }
            else {
                cout << "\n  Te faltan " << faltan << " puntos." << endl;
                cout << "  Necesitas aprox. " << necesarias
                    << " sesiones de 25 pts para llegar." << endl;
            }

            cout << endl;
            pausar();
            break;
        }

        case 0: break;

        default:
            cout << "\n  Opcion invalida." << endl;
            pausar();
        }

    } while (opcion != 0);
}

#endif

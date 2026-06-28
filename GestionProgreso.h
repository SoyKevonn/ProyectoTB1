#pragma once
#ifndef GESTIONPROGRESO_H
#define GESTIONPROGRESO_H

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>   
#include <utility>   
#include <algorithm>  
#include "GestionUsuario.h"     
#include "GestionLecciones.h"   
#include "NivelInfo.h"
#include "EstadisticaUsuario.h"
#include "Fondo.h"


using namespace System;
using namespace std;

// Calcular cuántas sesiones faltan para llegar a la meta (Método recursivo lineal)
inline int sesionesParaObjetivo(int puntosFaltantes, int promedioPorSesion) {
    if (puntosFaltantes <= 0) return 0;
    return 1 + sesionesParaObjetivo(puntosFaltantes - promedioPorSesion, promedioPorSesion);
}

// ============================================================================
// --- ALGORITMO AVANZADO: QUICK SORT DESCENDENTE O(n log n) ---
// ============================================================================

inline int particionDescendente(vector<pair<int, string>>& arr, int bajo, int alto) {
    int pivote = arr[alto].first;
    int i = (bajo - 1);

    for (int j = bajo; j <= alto - 1; j++) {
        if (arr[j].first > pivote) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[alto]);
    return (i + 1);
}

inline void quickSortDescendente(vector<pair<int, string>>& arr, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionDescendente(arr, bajo, alto);
        quickSortDescendente(arr, bajo, pi - 1);
        quickSortDescendente(arr, pi + 1, alto);
    }
}

// ============================================================================
// --- APARTADO DE MANTENIMIENTO: INYECTOR Y PURGADOR DE ESTRÉS ---
// ============================================================================

// 1. FUNCIÓN PARA GENERAR LA PRUEBA DE ESTRÉS (10,000 usuarios ficticios)
inline void ejecutarPruebaEstres() {
    ofstream archivo("usuarios.txt", ios::app); 
    if (!archivo.is_open()) {
        cout << "\n  [Error] No se pudo acceder al archivo usuarios.txt." << endl;
        pausar();
        return;
    }

    for (int i = 0; i < 10000; i++) {
        int idFicticio = 30000 + i;
        int puntosAzar = rand() % 4000;
        int nivelCalculado = (puntosAzar < 100) ? 1 : (puntosAzar < 300) ? 2 : (puntosAzar < 600) ? 3 : 4;

        // Construcción compatible con el parser de la HashTable de tu grupo
        archivo << idFicticio << "|EstudianteFicticio_" << i << "|StressTest|Espaniol|stub_" << i
            << "@linguadevs.com|" << nivelCalculado << "|" << puntosAzar
            << "|Gratis|0|0|0000\r\n";
    }
    archivo.close();

    cout << "\n  === PRUEBA DE ESTRÉS GENERADA CON ÉXITO ===" << endl;
    cout << "  Se han inyectado 10,000 perfiles simulados en usuarios.txt." << endl;
    cout << "  [AVISO] Reinicie el programa para cargarlos en la HashTable." << endl;
    pausar();
}

// 2. FUNCIÓN PARA PURGAR / BORRAR LA PRUEBA DE ESTRÉS
inline void limpiarRegistrosDeEstres() {
    ifstream archivoLectura("usuarios.txt");
    if (!archivoLectura.is_open()) {
        cout << "\n  [Error] No se encontro el archivo usuarios.txt para limpiar." << endl;
        pausar();
        return;
    }

    vector<string> usuariosReales;
    string linea;
    int eliminados = 0;

    // Filtrado en memoria
    while (getline(archivoLectura, linea)) {
        if (linea.find("@linguadevs.com") != string::npos || linea.find("StressTest") != string::npos) {
            eliminados++;
        }
        else {
            if (!linea.empty()) {
                usuariosReales.push_back(linea);
            }
        }
    }
    archivoLectura.close();

    // Sobreescritura física truncada
    ofstream archivoEscritura("usuarios.txt", ios::trunc);
    if (!archivoEscritura.is_open()) {
        cout << "\n  [Error] Fallo critico de permisos al reescribir usuarios.txt." << endl;
        pausar();
        return;
    }

    for (size_t i = 0; i < usuariosReales.size(); i++) {
        archivoEscritura << usuariosReales[i] << "\n";
    }
    archivoEscritura.close();

    cout << "\n  === MANTENIMIENTO COMPLETADO CON ÉXITO ===" << endl;
    cout << "  Registros de estres purgados: " << eliminados << endl;
    cout << "  Cuentas legitimas preservadas: " << usuariosReales.size() << endl;
    cout << "  [AVISO] Reinicie el programa para limpiar la RAM de la HashTable." << endl;
    pausar();
}

// ============================================================================
// --- MENÚ PRINCIPAL DEL MÓDULO DE PROGRESO ---
// ============================================================================
inline void menuProgreso(GestionLecciones* gL) {

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

    NivelInfo<int>        nivelInfo;
    EstadisticaUsuario<int> stats;

    auto generarBarra = [](int p) {
        int tope = 1000;
        int barras = (p >= tope) ? 10 : (p * 10 / tope);
        cout << "  [";
        for (int i = 0; i < barras; i++)       cout << "#";
        for (int i = barras; i < 10; i++)      cout << "-";
        cout << "] " << p << " / " << tope << " pts" << endl;
        };

    auto puntosFaltantes = [](int meta, int actual) -> int {
        return (meta > actual) ? meta - actual : 0;
        };

    int opcion;
    do {
        limpiarPantalla();
        dibujarFondoProgreso();

        Console::SetCursorPosition(31, 11); cout << "  Usuario: " << u->getNombre() << " " << u->getApellido()
            << " [" << u->getSuscripcion()->getTipo() << "]" << endl;

        Console::SetCursorPosition(31, 13); cout << "1. Ver mi progreso" << endl;
        Console::SetCursorPosition(31, 14); cout << "2. Ver lecciones" << endl;
        Console::SetCursorPosition(31, 15); cout << "3. Estadisticas" << endl;
        Console::SetCursorPosition(31, 16); cout << "4. Ranking de estudiantes (Top 10)" << endl;
        Console::SetCursorPosition(31, 17); cout << "5. Proyectar meta" << endl;
        Console::SetCursorPosition(31, 18); cout << "6. Herramientas de Mantenimiento y Estres" << endl;
        Console::SetCursorPosition(31, 19); cout << "0. Volver al menu principal" << endl;
        Console::SetCursorPosition(31, 20); cout << "Opcion: ";
        cin >> opcion; cin.ignore();

        switch (opcion) {

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

        case 2: {
            limpiarPantalla();
            cout << "\n  === LECCIONES ===" << endl << endl;
            int completadas = u->getLeccionesCompletadas();

            for (size_t i = 0; i < gL->cantidadLecciones(); i++) {
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

        case 3: {
            limpiarPantalla();
            cout << "\n  === ESTADISTICAS ===" << endl << endl;
            int completadas = u->getLeccionesCompletadas();
            if (completadas == 0) {
                cout << "  Aun no has completado ninguna leccion." << endl;
            }
            else {
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

        case 4: {
            limpiarPantalla();
            cout << "\n  === RANKING DE ESTUDIANTES (TOP 10) ===" << endl << endl;

            // Restricción perimetral: Solo cuentas Premium ven el Ranking Global
            if (u->getSuscripcion()->getTipo() != "Premium") {
                cout << "  [ACCESO RESTRINGIDO]" << endl;
                cout << "  Esta funcionalidad (Ranking Global) requiere una suscripcion Premium." << endl;
                cout << "  Tu estado actual es: " << u->getSuscripcion()->getTipo() << endl;
                cout << "\n  Adquiere un plan Premium en el modulo de Usuarios para desbloquearlo." << endl;
                pausar();
                break;
            }

            vector<Usuario<int>*> todos = tablaUsuarios.obtenerValores();
            int n = (int)todos.size();

            if (n == 0) {
                cout << "  No hay usuarios registrados." << endl;
                pausar();
                break;
            }

            vector<pair<int, string>> tabla;
            for (int i = 0; i < n; i++) {
                Usuario<int>* usr = todos[i];
                if (usr) {
                    tabla.push_back({ usr->getPuntos(), usr->getNombre() + " " + usr->getApellido() });
                }
            }

            if (!tabla.empty()) {
                quickSortDescendente(tabla, 0, (int)tabla.size() - 1);
            }

            int maxTop = ((int)tabla.size() < 10) ? (int)tabla.size() : 10;
            for (int i = 0; i < maxTop; i++) {
                cout << "  " << i + 1 << ". " << tabla[i].second
                    << " - " << tabla[i].first << " pts";

                if (tabla[i].second == u->getNombre() + " " + u->getApellido()) {
                    cout << "  <- tu";
                }
                cout << endl;
            }
            cout << endl;
            pausar();
            break;
        }

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

        case 6: {
            // SUBMENÚ DE MANTENIMIENTO AVANZADO
            limpiarPantalla();
            cout << "\n  === PANEL DE CONTROL Y SIMULACIÓN ===" << endl << endl;
            cout << "  1. Generar Prueba de Estres (Inyectar 10,000 usuarios)" << endl;
            cout << "  2. Eliminar usuarios de Prueba de Estres (Limpiar dataset)" << endl;
            cout << "  0. Regresar" << endl;
            cout << "\n  Seleccione una opcion: ";
            int subOpcion; cin >> subOpcion; cin.ignore();

            if (subOpcion == 1) {
                ejecutarPruebaEstres();
                return; 
            }
            else if (subOpcion == 2) {
                limpiarRegistrosDeEstres();
                return; 
            }
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
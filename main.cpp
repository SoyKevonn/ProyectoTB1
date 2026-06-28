#include <iostream>
#include <limits>
#include "GestionUsuario.h"
#include "GestionLecciones.h"
#include "GestionProgreso.h"
#include "Fondo.h"
using namespace std;

GestionLecciones* gL = new GestionLecciones();

void pausar();
void limpiarPantalla();

int main() {
    cargarUsuarios();
    srand(time(NULL));

    int opcion = -1;
    do {
        limpiarPantalla();
        dibujarFondoMenu();

        // Kevin
        Console::SetCursorPosition(17, 13); cout << "  1. Usuarios";
        // Salvador
        Console::SetCursorPosition(17, 18); cout << "  2. Lecciones y ejercicios";
        // Alfredo
        Console::SetCursorPosition(17, 23); cout << "  3. Progreso";

        char tecla = _getch();

        if (tecla >= '0' && tecla <= '3')
            opcion = tecla - '0';
        else
            opcion = -1;

        switch (opcion) {
        case 1:
            menuUsuarios();
            break;
        case 2:
            limpiarPantalla();
            gL->mostrarMenu();
            break;
        case 3:
            limpiarPantalla();
            menuProgreso(gL);
            break;
        case 0:
            break;
        default:
            break;
        }

    } while (opcion != 0);

    // liberar memoria de usuarios usando tu Tabla Hash
    vector<Usuario<int>*> todosLosUsuarios = tablaUsuarios.obtenerValores();
    for (size_t i = 0; i < todosLosUsuarios.size(); i++) {
        delete todosLosUsuarios[i];
    }

    delete gL;
    return 0;
}
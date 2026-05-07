#include <iostream>
#include <limits>
#include "GestionUsuario.h"
#include "GestionLecciones.h"
using namespace std;

GestionLecciones* gL = new GestionLecciones();

void pausar();
void limpiarPantalla();

int main() {
    cargarUsuarios();

    srand(time(NULL));

    int opcion;
    do {
        limpiarPantalla();
        mostrarEncabezado();
        //Kevin
        cout << "  1. Usuarios y recompensas" << endl;
        //Salvador
        cout << "  2. Lecciones y ejercicios" << endl;
        //Alfredo
        cout << "  3. Progreso" << endl;
        cout << "  0. Salir" << endl;
        cout << endl << "  Opcion: "; cin >> opcion; cin.ignore();

        switch (opcion) {
        case 1: menuUsuarios(); break;
        case 2:
            limpiarPantalla();
            gL->mostrarMenu();
            break;
        case 3:
            limpiarPantalla();
            // pendiente Alfredo
            break;
        case 0:
            cout << endl << "  Hasta luego." << endl;
            break;
        default:
            cout << endl << "  Opcion invalida." << endl;
            pausar();
        }
    } while (opcion != 0);

    // liberar memoria de usuarios
    for (uint i = 0; i < listaUsuarios.longitud(); i++)
        delete listaUsuarios.obtenerPos(i);

    delete gL;

    return 0;
}
#include <iostream>
#include <limits>
#include "GestionUsuario.h"
using namespace std;

void pausar();
void limpiarPantalla();

int main() {
    cargarUsuarios();

    int opcion;
    do {
        limpiarPantalla();
        mostrarEncabezado();
        //Kevin
        cout << "  1. Usuarios y recompensas" << endl;
        //Salvador
        cout << "  2. Lecciones y ejercicios" << endl;
        //Alfredo
        cout << "  3. Idiomas y progreso" << endl;
        cout << "  0. Salir" << endl;
        cout << endl <<"  Opcion: "; cin >> opcion;

        switch (opcion) {
        case 1: menuUsuarios(); break;
        case 2:
            limpiarPantalla();
            //pendiente
            break;
        case 3:
            limpiarPantalla();
            //Pendiente
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

    return 0;
}

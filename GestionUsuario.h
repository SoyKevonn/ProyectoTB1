#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include "Lista.h"
#include "Usuario.h"
#include "Recompensa.h"
using namespace std;

// lista global de usuarios y contador de ids
Lista<Usuario<int>*> listaUsuarios;
int contadorIdUsuario = 1;
int contadorIdRecompensa = 1;

// precionar enter para continuar. ayuda de ia
void pausar() {
    cout << "\n  presione enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void limpiarPantalla() {
    system("cls");
}

void mostrarEncabezado() {
    cout << "============================================" << endl;
    cout << "     LINGUATRACK - Aprende un Idioma       " << endl;
    cout << "============================================" << endl;
}

// recompensas por defecto que se asignan a cada usuario nuevo
void asignarRecompensasBase(Usuario<int>* u) {
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Principiante", "Completa tu primer ejercicio", 50));
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Constante", "Acumula 200 puntos", 200));
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Dedicado", "Alcanza 600 puntos", 600));
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Experto", "Acumula 1000 puntos", 1000));
    u->ordenarRecompensasPorPuntos();
}

// cargar usuarios desde archivo al iniciar
void cargarUsuarios() {
    ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) return;

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        Usuario<int>* u = Usuario<int>::cargarDesdeLinea(linea);
        if (u != nullptr) {
            asignarRecompensasBase(u);
            listaUsuarios.agregaFinal(u);
            if (u->getId() >= contadorIdUsuario)
                contadorIdUsuario = u->getId() + 1;
        }
    }
    archivo.close();
}

// buscar por id
Usuario<int>* buscarUsuarioPorId(int id) {
    for (uint i = 0; i < listaUsuarios.longitud(); i++) {
        Usuario<int>* u = listaUsuarios.obtenerPos(i);
        if (u && u->getId() == id) return u;
    }
    return nullptr;
}

// reescribir el archivo con info actual
void reescribirArchivoUsuarios() {
    ofstream archivo("usuarios.txt", ios::trunc);
    archivo.close();
    for (uint i = 0; i < listaUsuarios.longitud(); i++) {
        Usuario<int>* u = listaUsuarios.obtenerPos(i);
        if (u) u->guardarEnArchivo("usuarios.txt");
    }
}

// registrar nuevo usuario
void registrarUsuario() {
    limpiarPantalla();
    cout << "  -- Registrar usuario --" << endl << endl;

    string nombre, apellido, idioma, correo;
    cout << "  Nombre      : "; getline(cin, nombre);
    cout << "  Apellido    : "; getline(cin, apellido);
    cout << "  Idioma nat. : "; getline(cin, idioma);
    cout << "  Correo      : "; getline(cin, correo);

    Usuario<int>* nuevo = new Usuario<int>(
        contadorIdUsuario++, nombre, apellido, idioma, correo
    );
    asignarRecompensasBase(nuevo);
    listaUsuarios.agregaFinal(nuevo);
    nuevo->guardarEnArchivo("usuarios.txt");

    cout << endl << "  Usuario registrado correctamente." << endl;
    nuevo->mostrar();
    pausar();
}

// listar los usuarios
void listarUsuarios() {
    limpiarPantalla();
    cout << "  -- Lista de usuarios --" << endl << endl;

    if (listaUsuarios.esVacia()) {
        cout << "  No hay usuarios registrados." << endl;
    }
    else {
        for (uint i = 0; i < listaUsuarios.longitud(); i++) {
            Usuario<int>* u = listaUsuarios.obtenerPos(i);
            if (u) u->mostrar();
        }
    }
    pausar();
}

// agregar puntos
void agregarPuntosUsuario() {
    limpiarPantalla();
    cout << "  -- Agregar puntos --" << endl << endl;

    int id, puntos;
    cout << "  ID del usuario  : "; cin >> id;
    cout << "  Puntos a agregar: "; cin >> puntos;
    cin.ignore();

    Usuario<int>* u = buscarUsuarioPorId(id);
    if (u == nullptr) {
        cout << endl << "  Ssuario no encontrado." << endl;
    }
    else {
        u->agregarPuntos(puntos);
        reescribirArchivoUsuarios();
        cout << endl << "  Puntos actualizados." << endl;
        u->mostrar();
    }
    pausar();
}

// buscar recompensa con usuario
void buscarRecompensaUsuario() {
    limpiarPantalla();
    cout << "  -- Buscar recompensa --" << endl << endl;

    int id;
    string nombreRecompensa;
    cout << "  ID del usuario    : "; cin >> id; cin.ignore();
    cout << "  Nombre recompensa : "; getline(cin, nombreRecompensa);

    Usuario<int>* u = buscarUsuarioPorId(id);
    if (u == nullptr) {
        cout << endl << "  Usuario no encontrado." << endl;
    }
    else {
        Recompensa<int>* r = u->buscarRecompensa(nombreRecompensa);
        if (r == nullptr)
            cout << endl << "  Recompensa no encontrada." << endl;
        else {
            cout << endl << "  Recompensa encontrada:" << endl;
            r->mostrar();
        }
    }
    pausar();
}

// tabla x nivel
void mostrarTablaDeNiveles() {
    limpiarPantalla();
    cout << "  -- Puntos necesarios por nivel --" << endl << endl;

    Usuario<int> temp;
    for (int n = 1; n <= 5; n++) {
        cout << "  Nivel " << n << " : " << temp.puntosParaNivel(n) << " puntos" << endl;
    }
    pausar();
}

// elimnar usuario id
void eliminarUsuario() {
    limpiarPantalla();
    cout << "  -- Eliminar usuario --" << endl << endl;

    int id;
    cout << "  ID del usuario: "; cin >> id; cin.ignore();

    for (uint i = 0; i < listaUsuarios.longitud(); i++) {
        Usuario<int>* u = listaUsuarios.obtenerPos(i);
        if (u && u->getId() == id) {
            listaUsuarios.eliminaPos(i);
            delete u;
            reescribirArchivoUsuarios();
            cout << endl << "  Usuario eliminado correctamente." << endl;
            pausar();
            return;
        }
    }
    cout << endl << "  Usuario no encontrado." << endl;
    pausar();
}

// modificar datos de un usuario existente
void modificarUsuario() {
    limpiarPantalla();
    cout << "  -- Modificar usuario --" << endl << endl;

    int id;
    cout << "  ID del usuario: "; cin >> id; cin.ignore();

    Usuario<int>* u = buscarUsuarioPorId(id);
    if (u == nullptr) {
        cout << endl << "  Ssuario no encontrado." << endl;
        pausar();
        return;
    }

    cout << endl;
    u->resumen();
    cout << endl;
    cout << "  Que dato desea modificar?" << endl;
    cout << "  1. Nombre" << endl;
    cout << "  2. Apellido" << endl;
    cout << "  3. Idioma nativo" << endl;
    cout << "  4. Correo" << endl;
    cout << "  0. Cancelar" << endl;
    cout << endl << "  Opcion: ";

    int op; cin >> op;
    string nuevoValor;

    switch (op) {
    case 1:
        cout << "  Nuevo nombre   : "; getline(cin, nuevoValor);
        u->setNombre(nuevoValor);
        break;
    case 2:
        cout << "  Nuevo apellido : "; getline(cin, nuevoValor);
        u->setApellido(nuevoValor);
        break;
    case 3:
        cout << "  Nuevo idioma   : "; getline(cin, nuevoValor);
        u->setIdiomaNativo(nuevoValor);
        break;
    case 4:
        cout << "  Nuevo correo   : "; getline(cin, nuevoValor);
        u->setCorreo(nuevoValor);
        break;
    case 0:
        return;
    default:
        cout << endl << "  Opcion invalida." << endl;
        pausar();
        return;
    }

    reescribirArchivoUsuarios();
    cout << endl << "  Usuario actualizado correctamente." << endl;
    u->mostrar();
    pausar();
}

// ranking de usuarios ordenados por puntos usando insertion sort — O(n^2)
void rankingUsuarios() {
    limpiarPantalla();
    cout << "  -- Ranking de usuarios --" << endl << endl;

    if (listaUsuarios.esVacia()) {
        cout << "  No hay usuarios registrados." << endl;
        pausar();
        return;
    }

    // copiar punteros a vector para ordenar sin modificar la lista original
    vector<Usuario<int>*> vec;
    for (uint i = 0; i < listaUsuarios.longitud(); i++)
        vec.push_back(listaUsuarios.obtenerPos(i));

    // insertion sort descendente por puntos totales
    int n = (int)vec.size();
    for (int i = 1; i < n; i++) {
        Usuario<int>* key = vec[i];
        int j = i - 1;
        while (j >= 0 && vec[j]->getPuntos() < key->getPuntos()) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = key;
    }

    // mostrar ranking con posicion
    for (int i = 0; i < n; i++) {
        cout << "  #" << (i + 1) << "  ";
        vec[i]->resumen();
    }

    pausar();
}

// menu
void menuUsuarios() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "  -- Gestion de usuarios --" << endl << endl;
        cout << "  1. Registrar usuario" << endl;
        cout << "  2. Listar usuarios" << endl;
        cout << "  3. Agregar puntos" << endl;
        cout << "  4. Buscar recompensa" << endl;
        cout << "  5. Tabla de niveles" << endl;
        cout << "  6. Eliminar usuario" << endl;
        cout << "  7. Modificar usuario" << endl;
        cout << "  8. Ranking de usuarios" << endl;
        cout << "  0. Volver" << endl;
        cout << endl << "  Opcion: "; cin >> opcion; cin.ignore();

        switch (opcion) {
        case 1: registrarUsuario();        break;
        case 2: listarUsuarios();          break;
        case 3: agregarPuntosUsuario();    break;
        case 4: buscarRecompensaUsuario(); break;
        case 5: mostrarTablaDeNiveles();   break;
        case 6: eliminarUsuario();         break;
        case 7: modificarUsuario();        break;
        case 8: rankingUsuarios();         break;
        case 0: break;
        default:
            cout << endl << "  opcion invalida." << endl;
            pausar();
        }
    } while (opcion != 0);
}

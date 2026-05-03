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

Lista<Usuario<int>*> listaUsuarios;
int contadorIdUsuario = 1;
int contadorIdRecompensa = 1;

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

void asignarRecompensasBase(Usuario<int>* u) {
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Principiante", "Completa tu primer ejercicio", 50));
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Constante", "Acumula 200 puntos", 200));
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Dedicado", "Alcanza 600 puntos", 600));
    u->agregarRecompensa(new Recompensa<int>(contadorIdRecompensa++, "Experto", "Acumula 1000 puntos", 1000));
    u->ordenarRecompensasPorPuntos();
}

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

Usuario<int>* buscarUsuarioPorId(int id) {
    for (uint i = 0; i < listaUsuarios.longitud(); i++) {
        Usuario<int>* u = listaUsuarios.obtenerPos(i);
        if (u && u->getId() == id) return u;
    }
    return nullptr;
}

void reescribirArchivoUsuarios() {
    ofstream archivo("usuarios.txt", ios::trunc);
    archivo.close();
    for (uint i = 0; i < listaUsuarios.longitud(); i++) {
        Usuario<int>* u = listaUsuarios.obtenerPos(i);
        if (u) u->guardarEnArchivo("usuarios.txt");
    }
}

void registrarUsuario() {
    limpiarPantalla();
    cout << "  -- Registrar usuario --" << endl << endl;

    string nombre, apellido, idioma, correo;
    int tipoSub;
    cout << "  Nombre      : "; getline(cin, nombre);
    cout << "  Apellido    : "; getline(cin, apellido);
    cout << "  Idioma nat. : "; getline(cin, idioma);
    cout << "  Correo      : "; getline(cin, correo);
    cout << "  Suscripcion (1: Gratis, 2: Premium): "; cin >> tipoSub;
    cin.ignore();

    Usuario<int>* nuevo = new Usuario<int>(
        contadorIdUsuario++, nombre, apellido, idioma, correo
    );

    if (tipoSub == 2) {
        nuevo->getSuscripcion()->setTipo("Premium");
        nuevo->getSuscripcion()->setMultiplicador(2.0);
        nuevo->enviarNotificacion("Bienvenido a Linguatrack Premium!", "Sistema");
    }

    asignarRecompensasBase(nuevo);
    listaUsuarios.agregaFinal(nuevo);
    nuevo->guardarEnArchivo("usuarios.txt");

    cout << endl << "  Usuario registrado correctamente." << endl;
    nuevo->mostrar();
    pausar();
}

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

void agregarPuntosUsuario() {
    limpiarPantalla();
    cout << "  -- Agregar puntos --" << endl << endl;

    int id, puntos;
    cout << "  ID del usuario  : "; cin >> id;
    cout << "  Puntos a agregar: "; cin >> puntos;
    cin.ignore();

    Usuario<int>* u = buscarUsuarioPorId(id);
    if (u == nullptr) {
        cout << endl << "  Usuario no encontrado." << endl;
    }
    else {
        u->agregarPuntos(puntos);
        reescribirArchivoUsuarios();
        cout << endl << "  Puntos actualizados (Se aplico multiplicador si es Premium)." << endl;
        u->mostrar();
    }
    pausar();
}

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

void mostrarTablaDeNiveles() {
    limpiarPantalla();
    cout << "  -- Puntos necesarios por nivel --" << endl << endl;

    Usuario<int> temp;
    for (int n = 1; n <= 5; n++) {
        cout << "  Nivel " << n << " : " << temp.puntosParaNivel(n) << " puntos" << endl;
    }
    pausar();
}

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

void modificarUsuario() {
    limpiarPantalla();
    cout << "  -- Modificar usuario --" << endl << endl;

    int id;
    cout << "  ID del usuario: "; cin >> id; cin.ignore();

    Usuario<int>* u = buscarUsuarioPorId(id);
    if (u == nullptr) {
        cout << endl << "  Usuario no encontrado." << endl;
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
    cout << "  5. Cambiar Suscripcion (Premium/Gratis)" << endl;
    cout << "  6. Aumentar Racha de estudio (+1)" << endl;
    cout << "  7. Restar vida por error (-1)" << endl;
    cout << "  0. Cancelar" << endl;
    cout << endl << "  Opcion: ";

    int op; cin >> op; cin.ignore();
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
    case 5:
        if (u->getSuscripcion()->getTipo() == "Gratis") {
            u->getSuscripcion()->setTipo("Premium");
            u->getSuscripcion()->setMultiplicador(2.0);
            u->enviarNotificacion("Suscripcion mejorada a Premium", "Sistema");
            cout << "  Suscripcion mejorada a Premium." << endl;
        }
        else {
            u->getSuscripcion()->setTipo("Gratis");
            u->getSuscripcion()->setMultiplicador(1.0);
            u->enviarNotificacion("Suscripcion cambiada a Gratis", "Sistema");
            cout << "  Suscripcion cambiada a Gratis." << endl;
        }
        break;
    case 6:
        u->getPerfil()->aumentarRacha();
        cout << "  Racha aumentada a " << u->getPerfil()->getRacha() << " dias." << endl;
        break;
    case 7:
        u->getPerfil()->perderVida();
        cout << "  Vidas restantes: " << u->getPerfil()->getVidas() << endl;
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

void rankingUsuarios() {
    limpiarPantalla();
    cout << "  -- Ranking de usuarios --" << endl << endl;

    if (listaUsuarios.esVacia()) {
        cout << "  No hay usuarios registrados." << endl;
        pausar();
        return;
    }

    vector<Usuario<int>*> vec;
    for (uint i = 0; i < listaUsuarios.longitud(); i++)
        vec.push_back(listaUsuarios.obtenerPos(i));

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

    for (int i = 0; i < n; i++) {
        cout << "  #" << (i + 1) << "  ";
        vec[i]->resumen();
    }

    pausar();
}

void enviarNotificacionGlobal() {
    limpiarPantalla();
    cout << "  -- Enviar Notificacion Global --" << endl << endl;

    if (listaUsuarios.esVacia()) {
        cout << "  No hay usuarios registrados." << endl;
        pausar();
        return;
    }

    string mensaje;
    cout << "  Escriba el mensaje para todos: "; getline(cin, mensaje);

    for (uint i = 0; i < listaUsuarios.longitud(); i++) {
        Usuario<int>* u = listaUsuarios.obtenerPos(i);
        if (u) u->enviarNotificacion(mensaje, "Global");
    }

    cout << endl << "  Notificacion enviada a todos los usuarios." << endl;
    pausar();
}

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
        cout << "  7. Modificar usuario / Jugar" << endl;
        cout << "  8. Ranking de usuarios" << endl;
        cout << "  9. Enviar Notificacion Global" << endl;
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
        case 9: enviarNotificacionGlobal(); break;
        case 0: break;
        default:
            cout << endl << "  opcion invalida." << endl;
            pausar();
        }
    } while (opcion != 0);
}
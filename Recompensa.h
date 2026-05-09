#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

// entidad que representa un logro que puede obtener un usuario
// al acumular puntos en la plataforma
template <typename TId>
class Recompensa
{
private:
    TId    id;
    string nombre;
    string descripcion;
    int    puntosRequeridos;
    bool   obtenida;

public:
    // constructores
    Recompensa()
        : id(TId()), nombre(""), descripcion(""), puntosRequeridos(0), obtenida(false) {}

    Recompensa(TId id, string nombre, string descripcion, int puntosRequeridos)
        : id(id), nombre(nombre), descripcion(descripcion),
        puntosRequeridos(puntosRequeridos), obtenida(false) {}

    // getters
    TId    getId() { return id; }
    string getNombre() { return nombre; }
    string getDescripcion() { return descripcion; }
    int    getPuntosRequeridos() { return puntosRequeridos; }
    bool   estaObtenida() { return obtenida; }

    // setters
    void setNombre(string n) { nombre = n; }
    void setDescripcion(string d) { descripcion = d; }
    void setPuntosRequeridos(int p) { puntosRequeridos = p; }
    void setObtenida(bool o) { obtenida = o; }

    // intenta desbloquear la recompensa si el usuario tiene puntos suficientes
    // retorna true si se desbloqueó en esta llamada
    bool intentarDesbloquear(int puntosUsuario) {
        // O(1) - Evaluación de condiciones lógicas simples (comparación de booleanos y enteros)
        if (!obtenida && puntosUsuario >= puntosRequeridos) {
            obtenida = true; // O(1) - Asignación directa de un valor a una variable
            return true;     // O(1) - Fin de la ejecución y retorno
        }
        return false;        // O(1) - Fin de la ejecución y retorno

        /*
          PROCESO BIG O:
          T(n) = O(1) + O(1) + O(1)
          T(n) = O(3)
          T(n) = O(1) Constante
        */
    }

    // muestra la recompensa en consola con su estado
    void mostrar() {
        cout << "    [" << (obtenida ? "X" : " ") << "] "
            << nombre << " - " << descripcion
            << " (requiere: " << puntosRequeridos << " pts)" << endl;
    }

    // escribe la recompensa en formato: id|nombre|descripcion|puntos|obtenida
    string toString() {
        ostringstream oss;
        oss << id;
        return oss.str() << "|" << nombre << "|" << descripcion << "|"
            << to_string(puntosRequeridos) << "|" << (obtenida ? "1" : "0");
    }

    bool operator==(const Recompensa& otro) const { return id == otro.id; }
    bool operator!=(const Recompensa& otro) const { return id != otro.id; }
};

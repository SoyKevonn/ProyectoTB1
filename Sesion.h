#pragma once
#ifndef SESION_H
#define SESION_H
#include <string>

using namespace std;

template <class T>
class Sesion {
private:
    T puntosGanados;
    string fecha;
public:
    Sesion(T p, std::string f) : puntosGanados(p), fecha(f) {}
    T getPuntos() { return puntosGanados; }
    string getFecha() { return fecha; }

};
#endif
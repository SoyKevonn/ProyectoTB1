#pragma once
#include <string>
using namespace std;

template <typename T>
class Suscripcion {
private:
    T id;
    string tipo; //gratis o premium
    double multiplicadorXP;
    bool activa;

public:
    Suscripcion() : id(T()), tipo("Gratis"), multiplicadorXP(1.0), activa(true) {}
    Suscripcion(T id, string tipo, double mult) : id(id), tipo(tipo), multiplicadorXP(mult), activa(true) {}

    T getId() { return id; }
    string getTipo() { return tipo; }
    double getMultiplicador() { return multiplicadorXP; }
    bool estaActiva() { return activa; }

    void setTipo(string t) { tipo = t; }
    void setMultiplicador(double m) { multiplicadorXP = m; }
    void setActiva(bool a) { activa = a; }
};
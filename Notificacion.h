#pragma once
#include <string>
#include <iostream>
using namespace std;

class Notificacion {
private:
    string mensaje;
    string fechaEnvio;
    bool leida;

public:
    Notificacion() : mensaje(""), fechaEnvio(""), leida(false) {}
    Notificacion(string m, string f) : mensaje(m), fechaEnvio(f), leida(false) {}

    string getMensaje() { return mensaje; }
    bool estaLeida() { return leida; }

    void marcarComoLeida() { leida = true; }

    void mostrar() {
        cout << "    [" << (leida ? "v" : "!") << "] " << mensaje << " (" << fechaEnvio << ")" << endl;
    }
};
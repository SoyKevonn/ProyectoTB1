#pragma once
#include <string>
using namespace std;

class Perfil {
private:
    int rachaDias;
    int vidas;
    string biografia;

public:
    Perfil() : rachaDias(0), vidas(5), biografia("Hola! Estoy aprendiendo.") {}

    int getRacha() { return rachaDias; }
    int getVidas() { return vidas; }
    string getBio() { return biografia; }

    void setRacha(int r) { rachaDias = r; }
    void setVidas(int v) { vidas = v; }
    void setBio(string b) { biografia = b; }

    void aumentarRacha() { rachaDias++; }
    void perderVida() { if (vidas > 0) vidas--; }
};
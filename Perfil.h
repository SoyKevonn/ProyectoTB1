#pragma once
#include <string>
using namespace std;

class Perfil {
private:
    int rachaDias;
    string biografia;

public:
    Perfil() : rachaDias(0), biografia("Hola! Estoy aprendiendo.") {}

    int getRacha() { return rachaDias; }
    string getBio() { return biografia; }

    void setRacha(int r) { rachaDias = r; }
    void setBio(string b) { biografia = b; }

    void aumentarRacha() { rachaDias++; } // O(1) Constante
};
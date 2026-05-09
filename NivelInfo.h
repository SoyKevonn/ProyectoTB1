#pragma once
#ifndef NIVELINFO_H
#define NIVELINFO_H
#include <string>

using namespace std;

template <class T>
class NivelInfo {
public:
    string obtenerRango(T puntos) {
        if (puntos < 100) return "Novato";
        if (puntos < 500) return "Aprendiz";
        if (puntos < 1000) return "Intermedio";
        return "Maestro";
    }
};

#endif
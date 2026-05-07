#pragma once
#ifndef NIVELINFO_H
#define NIVELINFO_H
#include <string>

template <class T>
class NivelInfo {
public:
    std::string obtenerRango(T puntos) {
        if (puntos < 100) return "Novato";
        if (puntos < 500) return "Aprendiz";
        if (puntos < 1000) return "Intermedio";
        return "Maestro";
    }
};

#endif
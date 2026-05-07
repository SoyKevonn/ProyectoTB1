#pragma once
#ifndef PROGRESO_H
#define PROGRESO_H

template <class T>
class Progreso {
private:
    T puntosTotales;
    int leccionesCompletadas;
public:
    Progreso(T pts = 0) : puntosTotales(pts), leccionesCompletadas(0) {}
    void actualizarProgreso(T pts) { puntosTotales += pts; leccionesCompletadas++; }
    T getPuntos() { return puntosTotales; }
   
};
#endif
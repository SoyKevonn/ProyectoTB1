#pragma once
#ifndef HISTORIALSESION_H
#define HISTORIALSESION_H

#include "Cola.h"     
#include "Sesion.h"   

template <class T>
class HistorialSesion {
private:
    Cola<Sesion<T>*> colaDeSesiones;
    int totalSesiones;

public:
    HistorialSesion() : totalSesiones(0) {}


    void agregarSesionAlHistorial(Sesion<T>* nuevaSesion) {
        colaDeSesiones.encolar(nuevaSesion);
        totalSesiones++;
    }

    int getCantidadSesiones() { return totalSesiones; }
};

#endif
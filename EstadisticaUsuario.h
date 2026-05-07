#pragma once
#ifndef ESTADISTICA_H
#define ESTADISTICA_H

template <class T>
class EstadisticaUsuario {
public:
    // recorre el arreglo una vez para sumar
    T calcularPromedio(T puntos[], int n) {
        if (n == 0) return 0;
        T suma = 0;
        for (int i = 0; i < n; i++) {
            suma += puntos[i];
        }
        return suma / n;
    }
};
#endif
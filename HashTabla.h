#pragma once
#include <iostream>
#include <vector>
#include "HashEntidad.h"

using namespace std;

// Convertido a template para aceptar <int, Usuario*>
template <class K, class V>
class HashTable {
private:
    HashEntidad<K, V>** table;
    int NumElements;
    int TableSize;
    HashEntidad<K, V>* DUMMY; // Usado para marcar elementos eliminados sin romper la Prueba Lineal

public:
    HashTable(int TableSize = 128) {
        this->TableSize = TableSize;
        table = new HashEntidad<K, V>* [TableSize];
        for (int i = 0; i < TableSize; i++) {
            table[i] = nullptr;
        }
        NumElements = 0;
        DUMMY = new HashEntidad<K, V>(K(), V());
    }

    ~HashTable() {
        for (int i = 0; i < TableSize; i++) {
            if (table[i] != nullptr && table[i] != DUMMY) {
                delete table[i];
            }
        }
        delete[] table;
        delete DUMMY;
    }

    // Direccionamiento según Prueba Lineal (Tu lógica adaptada)
    void insertar(K k, V v) {
        int base, step, hash;
        if (NumElements == TableSize) return; // Tabla llena

        base = k % TableSize;
        hash = base;
        step = 0;

        // Buscar espacio vacío, una tumba (DUMMY), o si la clave ya existe para actualizar
        while (table[hash] != nullptr && table[hash] != DUMMY && table[hash]->getKey() != k) {
            step++;
            hash = (base + step) % TableSize;
        }

        // Si ya existe, actualizamos su valor
        if (table[hash] != nullptr && table[hash] != DUMMY && table[hash]->getKey() == k) {
            table[hash]->setValue(v);
        }
        else {
            // Almacenamiento en la tabla
            table[hash] = new HashEntidad<K, V>(k, v);
            NumElements++;
        }
    }

    // Modificado para que retorne el Objeto (Usuario) en vez del índice
    V buscar(K k) {
        int step = 0;
        int base = k % TableSize;
        int hash = base;

        while (table[hash] != nullptr) {
            if (table[hash] != DUMMY && table[hash]->getKey() == k) {
                return table[hash]->getValue();
            }
            step++;
            hash = (base + step) % TableSize;
            if (step >= TableSize) break; // Evita bucle infinito si la tabla está llena
        }
        return nullptr; // No encontrado
    }

    // Método necesario para eliminar usuarios de la plataforma
    bool eliminar(K k) {
        int step = 0;
        int base = k % TableSize;
        int hash = base;

        while (table[hash] != nullptr) {
            if (table[hash] != DUMMY && table[hash]->getKey() == k) {
                delete table[hash];
                table[hash] = DUMMY; // Se marca como DUMMY para no romper el 'step' de búsqueda
                NumElements--;
                return true;
            }
            step++;
            hash = (base + step) % TableSize;
            if (step >= TableSize) break;
        }
        return false;
    }

    // Método auxiliar para recuperar todos los usuarios y poder guardarlos en el TXT
    vector<V> obtenerValores() {
        vector<V> valores;
        for (int i = 0; i < TableSize; i++) {
            if (table[i] != nullptr && table[i] != DUMMY) {
                valores.push_back(table[i]->getValue());
            }
        }
        return valores;
    }

    int size() {
        return TableSize;
    }
    int sizeActual() {
        return NumElements;
    }
};
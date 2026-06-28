#pragma once

// Convertido a template para cumplir la rúbrica
template <class K, class V>
class HashEntidad {
private:
    K key;
    V value;
public:
    HashEntidad(K k, V v) : key(k), value(v) {}

    K getKey() {
        return key;
    }
    V getValue() {
        return value;
    }
    void setValue(V v) {
        value = v;
    }
};
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lista.h"
#include "Recompensa.h"
#include "Suscripcion.h"
#include "Perfil.h"
#include "Notificacion.h"

using namespace std;

// entidad que representa a un estudiante registrado en la plataforma
// usa lista enlazada para manejar sus recompensas
// integrante: kevin
template <typename T>
class Usuario
{
private:
    T id;
    string nombre;
    string apellido;
    string idiomaNativo;
    string correo;
    int nivel;
    int puntosTotales;

    Lista<Recompensa<int>*> recompensas;
    Perfil* perfil;
    Suscripcion<int>* suscripcion;
    Lista<Notificacion*> notificaciones;

public:
    // constructores
    Usuario()
        : id(T()), nombre(""), apellido(""), idiomaNativo(""),
        correo(""), nivel(1), puntosTotales(0) {
        perfil = new Perfil();
        suscripcion = new Suscripcion<int>(1, "Gratis", 1.0);
    }

    Usuario(T id, string nombre, string apellido, string idiomaNativo, string correo)
        : id(id), nombre(nombre), apellido(apellido), idiomaNativo(idiomaNativo), correo(correo),
        nivel(1), puntosTotales(0) {
        perfil = new Perfil();
        suscripcion = new Suscripcion<int>(1, "Gratis", 1.0);
    }

    ~Usuario() {
        delete perfil;
        delete suscripcion;
        for (uint i = 0; i < recompensas.longitud(); i++) delete recompensas.obtenerPos(i);
        for (uint i = 0; i < notificaciones.longitud(); i++) delete notificaciones.obtenerPos(i);
    }

    // getters
    T      getId() { return id; }
    string getNombre() { return nombre; }
    string getApellido() { return apellido; }
    string getIdiomaNativo() { return idiomaNativo; }
    string getCorreo() { return correo; }
    int    getNivel() { return nivel; }
    int    getPuntos() { return puntosTotales; }

    // setters
    void setNombre(string n) { nombre = n; }
    void setApellido(string a) { apellido = a; }
    void setIdiomaNativo(string i) { idiomaNativo = i; }
    void setCorreo(string c) { correo = c; }
    void setNivel(int n) { nivel = n; }
    void setPuntosTotales(int p) { puntosTotales = p; }

    Perfil* getPerfil() { return perfil; }
    Suscripcion<int>* getSuscripcion() { return suscripcion; }

    void enviarNotificacion(string m, string f) {
        notificaciones.agregaFinal(new Notificacion(m, f));
    }

    // agrega una recompensa al catalogo del usuario — O(1)
    void agregarRecompensa(Recompensa<int>* r) {
        recompensas.agregaFinal(r);
    }

    // agrega puntos al usuario y revisa si sube de nivel — O(n)
    // lambda 1: calcula el nivel segun los puntos acumulados
    void agregarPuntos(int puntos) {
        if (puntos <= 0) return;

        // Aplicar multiplicador de suscripción
        int puntosFinales = puntos * suscripcion->getMultiplicador();
        puntosTotales += puntosFinales;

        auto calcularNivel = [](int pts) -> int {
            if (pts < 100) return 1;
            if (pts < 300) return 2;
            if (pts < 600) return 3;
            return 4;
            };

        int nuevoNivel = calcularNivel(puntosTotales);
        if (nuevoNivel > nivel) {
            nivel = nuevoNivel;
            enviarNotificacion("Felicidades! Subiste al nivel " + to_string(nivel), "Hoy");
        }
        verificarRecompensas();
    }

    // revisa cada recompensa y la desbloquea si el usuario tiene puntos — O(n)
    // lambda 2: notifica en consola cuando se desbloquea una recompensa
    void verificarRecompensas() {
        auto notificarDesbloqueo = [](const string& nombreR) {
            cout << "  *** Recompensa desbloqueada: " << nombreR << " ***" << endl;
            };

        for (uint i = 0; i < recompensas.longitud(); i++) {
            Recompensa<int>* r = recompensas.obtenerPos(i);
            if (r != nullptr && r->intentarDesbloquear(puntosTotales))
                notificarDesbloqueo(r->getNombre());
        }
    }

    // ayuda de ia
    // busca una recompensa por nombre sin distinguir mayusculas — O(n)
    // lambda 3: compara dos strings sin importar mayusculas/minusculas
    Recompensa<int>* buscarRecompensa(const string& nombreBuscado) {
        auto coincide = [](const string& a, const string& b) -> bool {
            if (a.size() != b.size()) return false;
            for (size_t i = 0; i < a.size(); i++) {
                if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
                    return false;
            }
            return true;
            };

        for (uint i = 0; i < recompensas.longitud(); i++) {
            Recompensa<int>* r = recompensas.obtenerPos(i);
            if (r != nullptr && coincide(r->getNombre(), nombreBuscado))
                return r;
        }
        return nullptr;
    }

    // calcula recursivamente los puntos necesarios para alcanzar un nivel
    // caso base: nivel 1 requiere 0 puntos — O(nivelObjetivo)
    int puntosParaNivel(int nivelObjetivo) {
        if (nivelObjetivo <= 1) return 0;
        return (nivelObjetivo - 1) * 100 + puntosParaNivel(nivelObjetivo - 1);
    }

    // ordena las recompensas por puntos requeridos usando insertion sort — O(n^2)
    void ordenarRecompensasPorPuntos() {
        int n = (int)recompensas.longitud();
        if (n <= 1) return;

        vector<Recompensa<int>*> vec;
        for (int i = 0; i < n; i++)
            vec.push_back(recompensas.obtenerPos(i));

        for (int i = 1; i < n; i++) {
            Recompensa<int>* key = vec[i];
            int j = i - 1;
            while (j >= 0 && vec[j]->getPuntosRequeridos() > key->getPuntosRequeridos()) {
                vec[j + 1] = vec[j];
                j--;
            }
            vec[j + 1] = key;
        }

        while (!recompensas.esVacia()) recompensas.eliminaInicial();
        for (int i = 0; i < (int)vec.size(); i++)
            recompensas.agregaFinal(vec[i]);
    }

    void resumen() {
        cout << "  [" << id << "] " << nombre << " " << apellido
            << " | Nivel " << nivel
            << " | " << puntosTotales << " pts" << endl;
    }

    // muestra los datos completos del usuario unificando perfil y recompensas
    void mostrar() {
        cout << "  === PERFIL DE " << nombre << " " << apellido << " (" << suscripcion->getTipo() << ") ===" << endl;
        cout << "  ID      : " << id << " | Correo: " << correo << " | Idioma: " << idiomaNativo << endl;
        cout << "  Vidas   : " << perfil->getVidas() << " | Racha: " << perfil->getRacha() << " dias" << endl;
        cout << "  Puntos  : " << puntosTotales << " | Nivel: " << nivel << endl;

        if (!notificaciones.esVacia()) {
            cout << "  Notificaciones recientes:" << endl;
            for (uint i = 0; i < notificaciones.longitud(); i++) notificaciones.obtenerPos(i)->mostrar();
        }

        cout << "  Recompensas (" << recompensas.longitud() << "):" << endl;
        for (uint i = 0; i < recompensas.longitud(); i++) {
            Recompensa<int>* r = recompensas.obtenerPos(i);
            if (r) r->mostrar();
        }
        cout << "  ======================================" << endl;
    }

    // guarda el usuario en un archivo de texto
    // formato: id|nombre|apellido|idiomaNativo|correo|nivel|puntos
    void guardarEnArchivo(const string& ruta) {
        ofstream archivo(ruta, ios::app);
        if (archivo.is_open()) {
            archivo << id << "|" << nombre << "|" << apellido << "|"
                << idiomaNativo << "|" << correo << "|"
                << nivel << "|" << puntosTotales << "|"
                << suscripcion->getTipo() << "|" << perfil->getRacha() << "|" << perfil->getVidas() << "\n";
            archivo.close();
        }
    }

    // carga un usuario desde una linea del archivo de texto
    static Usuario<int>* cargarDesdeLinea(const string& linea) {
        vector<string> partes;
        string token;
        for (char c : linea) {
            if (c == '|') { partes.push_back(token); token = ""; }
            else token += c;
        }
        partes.push_back(token);

        if (partes.size() < 7) return nullptr;

        Usuario<int>* u = new Usuario<int>(
            stoi(partes[0]), partes[1], partes[2], partes[3], partes[4]
        );
        u->setNivel(stoi(partes[5]));
        u->setPuntosTotales(stoi(partes[6]));

        // Cargar nuevos atributos si el archivo ya fue actualizado
        if (partes.size() >= 10) {
            if (partes[7] == "Premium") {
                u->getSuscripcion()->setTipo("Premium");
                u->getSuscripcion()->setMultiplicador(2.0);
            }
            u->getPerfil()->setRacha(stoi(partes[8]));
            u->getPerfil()->setVidas(stoi(partes[9]));
        }

        return u;
    }

    bool operator==(const Usuario& otro) const { return id == otro.id; }
    bool operator!=(const Usuario& otro) const { return id != otro.id; }
};
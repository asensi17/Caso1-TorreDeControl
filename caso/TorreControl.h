#pragma once

#include "Contexto.h"

#include <condition_variable>
#include <mutex>
#include <string>

class TorreControl {
public:
    TorreControl(const Contexto& ctx, int pistas, int puertas, int aviones);

    // Pistas (Persona 2)
    int pedirPistaAterrizaje(int idAvion);
    int pedirPistaDespegue(int idAvion);
    void soltarPista(int pista, int idAvion);

    // Puertas, panel y operaciones (Persona 3)
    void pedirPuerta(int idAvion);
    void soltarPuerta(int idAvion);
    void actualizarEstado(int idAvion, const std::string& estado);
    void registrarOperacion();
    long operaciones();
    void pantalla(int id);
    void operador();
    void cerrarPanel();

private:
    std::mutex _mPanel;
    std::condition_variable _cvPanel;
    bool _panelCerrado = false;
};

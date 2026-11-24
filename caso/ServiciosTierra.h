
#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <string>

#include "Estadisticas.h"
#include "Vigilante.h"

using namespace std;

class ServiciosTierra {

private:

    struct Servicio {
        int idAvion;
        string tipo;
        int duracionMs;
    };

    int capacidad;
    bool cerrada = false;

    queue<Servicio> cola;
    unordered_map<int, int> pendientes;

    mutex mtx;

    condition_variable hayEspacio;
    condition_variable hayTrabajo;
    condition_variable serviciosTerminados;

    Estadisticas& estadisticas;
    Vigilante& vigilante;

public:

    ServiciosTierra(int capacidad,
        Estadisticas& estadisticas,
        Vigilante& vigilante);

    void encargarYEsperarServicios(int idAvion);
    void ejecutarOperario();
    void cerrarCola();
};

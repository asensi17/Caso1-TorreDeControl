#pragma once

#include "Contexto.h"

#include <condition_variable>
#include <mutex>

class TorreControl;

class ServiciosTierra {
public:
    ServiciosTierra(const Contexto& ctx, int capacidad, int aviones, TorreControl& torre);

    // Cola de servicios (Persona 4)
    void encargarServicios(int idAvion);
    void esperarServicios(int idAvion);
    bool tienePendientes(int idAvion);
    void operario(int id);
    void cerrar();

private:
    std::mutex _m;
    std::condition_variable _cv;
    bool _cerrado = false;
};

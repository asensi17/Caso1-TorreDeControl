#include "ServiciosTierra.h"

#include "TorreControl.h"

ServiciosTierra::ServiciosTierra(const Contexto& ctx, int capacidad, int aviones, TorreControl& torre) {
    (void)ctx;
    (void)capacidad;
    (void)aviones;
    (void)torre;
}

void ServiciosTierra::encargarServicios(int idAvion) {
    (void)idAvion;  // TODO (Persona 4)
}

void ServiciosTierra::esperarServicios(int idAvion) {
    (void)idAvion;
}

bool ServiciosTierra::tienePendientes(int idAvion) {
    (void)idAvion;
    return false;
}

void ServiciosTierra::operario(int id) {
    (void)id;
    std::unique_lock<std::mutex> lock(_m);
    while (!_cerrado) {
        _cv.wait(lock);
    }
}

void ServiciosTierra::cerrar() {
    std::lock_guard<std::mutex> lock(_m);
    _cerrado = true;
    _cv.notify_all();
}

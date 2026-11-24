#include "Avion.h"

#include "Config.h"
#include "Estadisticas.h"
#include "ServiciosTierra.h"
#include "TorreControl.h"
#include "Traza.h"
#include "Vigilante.h"

#include <chrono>
#include <string>
#include <thread>

namespace {
constexpr int T_LLEGADA_MS = 2;
constexpr int T_ATERRIZAR_MS = 2;
constexpr int T_EMBARCAR_MS = 2;
constexpr int T_DESPEGAR_MS = 2;
}

Avion::Avion(int id, const Contexto& ctx, TorreControl& torre, ServiciosTierra& servicios)
    : _id(id),
      _ctx(ctx),
      _torre(torre),
      _servicios(servicios),
      _interbloqueo(ctx.config.fallo("interbloqueo")) {}

void Avion::ejecutar() {
    const std::string nombre = "avion " + std::to_string(_id);

    // 1. Llegada
    std::this_thread::sleep_for(std::chrono::milliseconds(T_LLEGADA_MS * _id));
    _torre.actualizarEstado(_id, "en aproximacion");
    _ctx.traza.evento(nombre, "en aproximacion");

    // 2 y 3. Puerta, pista y aterrizaje (con --fallo interbloqueo invierte el orden sin soltar pista)
    int pistaAterrizaje = 0;
    if (_interbloqueo) {
        pistaAterrizaje = _torre.pedirPistaAterrizaje(_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(T_ATERRIZAR_MS));
        _torre.pedirPuerta(_id);
    } else {
        _torre.pedirPuerta(_id);
        pistaAterrizaje = _torre.pedirPistaAterrizaje(_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(T_ATERRIZAR_MS));
    }
    _torre.soltarPista(pistaAterrizaje, _id);
    _torre.registrarOperacion();
    _ctx.vigilante.progreso();
    _torre.actualizarEstado(_id, "en puerta");
    _ctx.traza.evento(nombre, "en puerta");

    // 4. Servicios de tierra
    _servicios.encargarServicios(_id);
    _servicios.esperarServicios(_id);

    // 5. Embarque
    std::this_thread::sleep_for(std::chrono::milliseconds(T_EMBARCAR_MS));

    // 6. Despegue
    if (_servicios.tienePendientes(_id)) {
        _ctx.estadisticas.violacion(nombre + " despega con servicios pendientes");
    }
    int pistaDespegue = _torre.pedirPistaDespegue(_id);
    _torre.soltarPuerta(_id);
    std::this_thread::sleep_for(std::chrono::milliseconds(T_DESPEGAR_MS));
    _torre.soltarPista(pistaDespegue, _id);
    _torre.registrarOperacion();
    _ctx.vigilante.progreso();
    _torre.actualizarEstado(_id, "despegado");
    _ctx.traza.evento(nombre, "despegado");
}

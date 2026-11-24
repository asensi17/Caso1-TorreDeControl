#include "Avion.h"
#include "Config.h"
#include "Contexto.h"
#include "Estadisticas.h"
#include "Lanzador.h"
#include "ServiciosTierra.h"
#include "TorreControl.h"
#include "Traza.h"
#include "Vigilante.h"

#include <chrono>
#include <cstddef>
#include <string>
#include <thread>
#include <vector>

bool ejecutarSimulacion(const Config& config, Estadisticas& estadisticas, Traza& traza, const std::string& etiqueta) {
    int numAviones = config.entero("aviones", 30);
    int numPistas = config.entero("pistas", 2);
    int numPuertas = config.entero("puertas", 4);
    int capCola = config.entero("cola", 6);
    int numOperarios = config.entero("operarios", 3);
    int numPantallas = config.entero("pantallas", 3);

    Vigilante vigilante(std::chrono::milliseconds(config.entero("vigilante", 2000)), estadisticas, etiqueta);
    Contexto ctx{config, estadisticas, traza, vigilante};

    TorreControl torre(ctx, numPistas, numPuertas, numAviones);
    ServiciosTierra servicios(ctx, capCola, numAviones, torre);

    std::vector<Avion> listaAviones;
    listaAviones.reserve(static_cast<std::size_t>(numAviones));
    for (int i = 0; i < numAviones; ++i) {
        listaAviones.emplace_back(i, ctx, torre, servicios);
    }

    vigilante.iniciar();

    std::vector<std::thread> hilosOperarios;
    for (int i = 0; i < numOperarios; ++i) {
        hilosOperarios.emplace_back(&ServiciosTierra::operario, &servicios, i);
    }

    std::vector<std::thread> hilosPantallas;
    for (int i = 0; i < numPantallas; ++i) {
        hilosPantallas.emplace_back(&TorreControl::pantalla, &torre, i);
    }

    std::thread hiloOperador(&TorreControl::operador, &torre);

    std::vector<std::thread> hilosAviones;
    for (int i = 0; i < numAviones; ++i) {
        hilosAviones.emplace_back(&Avion::ejecutar, &listaAviones[static_cast<std::size_t>(i)]);
    }

    // Cierre ordenado: aviones -> cerrar cola y panel -> resto de hilos
    for (auto& t : hilosAviones) {
        t.join();
    }
    servicios.cerrar();
    torre.cerrarPanel();

    for (auto& t : hilosOperarios) {
        t.join();
    }
    for (auto& t : hilosPantallas) {
        t.join();
    }
    hiloOperador.join();

    vigilante.detener();

    // Invariante final
    long esperadas = 5L * numAviones;
    long hechas = torre.operaciones();
    if (hechas != esperadas) {
        estadisticas.violacion("operaciones = " + std::to_string(hechas) + ", esperado " +
                               std::to_string(esperadas));
    }

    return estadisticas.sinViolaciones();
}

int main(int argc, char* argv[]) {
    Config config(argc, argv);
    Lanzador lanzador("aeropuerto",
                      {"aviones", "pistas", "puertas", "cola", "operarios", "pantallas"},
                      {"carrera", "interbloqueo", "inanicion"});
    return lanzador.ejecutar(config, ejecutarSimulacion);
}

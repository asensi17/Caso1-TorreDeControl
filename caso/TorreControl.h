#pragma once

#include "Semaforo.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>

class Config;
class Estadisticas;
class Traza;
class Vigilante;

class TorreControl {
public:
    TorreControl(const Config& config, int numAviones, int pistas, int puertas,
                 Estadisticas& estadisticas, Traza& traza, Vigilante& vigilante);

    void ejecutarPantalla(int id);
    void ejecutarOperadorTorre();
    void actualizarEstadoAvion(int idAvion, const std::string& estado);
    void agregarAviso(const std::string& aviso);
    std::vector<std::string> leerPanel() const;
    std::vector<std::string> leerAvisos() const;
    void incrementarOperaciones();
    int obtenerOperaciones() const;
    void cerrar();

    void pedirPuerta();
    void liberarPuerta();
    int pedirPistaAterrizaje();
    int pedirPistaDespegue();
    void liberarPista(int pista);

private:
    int pedirPista(bool aterrizaje);

    Estadisticas& estadisticas_;
    Traza& traza_;
    Vigilante& vigilante_;
    const int maxPuertas_;
    Semaforo puertas_;
    std::mutex mutexPuertas_;
    int avionesEnPuerta_ = 0;

    std::mutex mutexPistas_;
    std::condition_variable hayPista_;
    std::vector<bool> pistasLibres_;
    int esperanAterrizar_ = 0;
    int esperanDespegar_ = 0;
    bool turnoAterrizaje_ = true;

    mutable std::shared_mutex mutexPanel_;
    std::vector<std::string> estadosAviones_;
    std::vector<std::string> avisosTorre_;
    std::atomic<bool> cerrado_{false};

    const bool modoCarrera_;
    std::atomic<int> operaciones_{0};
};

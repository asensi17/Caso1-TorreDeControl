#include "TorreControl.h"

#include "Config.h"
#include "Estadisticas.h"
#include "Traza.h"
#include "Vigilante.h"

#include <algorithm>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

TorreControl::TorreControl(const Config& config, int numAviones, int pistas, int puertas,
                           Estadisticas& estadisticas, Traza& traza, Vigilante& vigilante)
    : estadisticas_(estadisticas), traza_(traza), vigilante_(vigilante),
      maxPuertas_(puertas), puertas_(puertas),
      pistasLibres_(pistas, true), estadosAviones_(numAviones, "inicial"),
      modoCarrera_(config.fallo("carrera")) {}

void TorreControl::ejecutarPantalla(int id) {
    while (!cerrado_.load()) {
        const auto estados = leerPanel();
        const auto avisos = leerAvisos();
        if (traza_.activa()) {
            traza_.evento("pantalla " + std::to_string(id),
                          "vuelos consultados: " + std::to_string(estados.size()) +
                          ", avisos consultados: " + std::to_string(avisos.size()));
        }
        std::this_thread::sleep_for(1ms);
    }
}

void TorreControl::ejecutarOperadorTorre() {
    int numero = 0;
    while (!cerrado_.load()) {
        std::this_thread::sleep_for(30ms);
        if (cerrado_.load()) {
            break;
        }
        const auto aviso = "Aviso oficial de torre #" + std::to_string(numero++);
        agregarAviso(aviso);
        traza_.evento("operador de torre", aviso);
    }
}

void TorreControl::actualizarEstadoAvion(int idAvion, const std::string& estado) {
    std::unique_lock<std::shared_mutex> lock(mutexPanel_);
    estadosAviones_.at(idAvion) = estado;
}

void TorreControl::agregarAviso(const std::string& aviso) {
    std::unique_lock<std::shared_mutex> lock(mutexPanel_);
    avisosTorre_.push_back(aviso);
}

std::vector<std::string> TorreControl::leerPanel() const {
    std::shared_lock<std::shared_mutex> lock(mutexPanel_);
    return estadosAviones_;
}

std::vector<std::string> TorreControl::leerAvisos() const {
    std::shared_lock<std::shared_mutex> lock(mutexPanel_);
    return avisosTorre_;
}

void TorreControl::incrementarOperaciones() {
    if (modoCarrera_) {
        // Se separan la lectura y la escritura para permitir actualizaciones perdidas.
        const int anterior = operaciones_.load(std::memory_order_relaxed);
        std::this_thread::sleep_for(5ms);
        operaciones_.store(anterior + 1, std::memory_order_relaxed);
    } else {
        operaciones_.fetch_add(1, std::memory_order_relaxed);
    }
    vigilante_.progreso();
}

int TorreControl::obtenerOperaciones() const {
    return operaciones_.load(std::memory_order_relaxed);
}

void TorreControl::cerrar() {
    cerrado_.store(true);
}

void TorreControl::pedirPuerta() {
    const auto inicio = std::chrono::steady_clock::now();
    puertas_.acquire();
    estadisticas_.registrarEspera("avion (puerta)", std::chrono::steady_clock::now() - inicio);

    bool exceso = false;
    {
        std::lock_guard<std::mutex> lock(mutexPuertas_);
        ++avionesEnPuerta_;
        exceso = avionesEnPuerta_ > maxPuertas_;
    }

    if (exceso) {
        estadisticas_.violacion("Hay mas aviones en puertas que puertas disponibles");
    }
}

void TorreControl::liberarPuerta() {
    bool liberacionInvalida = false;
    {
        std::lock_guard<std::mutex> lock(mutexPuertas_);
        if (avionesEnPuerta_ <= 0) {
            liberacionInvalida = true;
        } else {
            --avionesEnPuerta_;
        }
    }

    if (liberacionInvalida) {
        estadisticas_.violacion("Se ha intentado liberar una puerta que no estaba ocupada");
        return;
    }

    puertas_.release();
}

int TorreControl::pedirPistaAterrizaje() {
    return pedirPista(true);
}

int TorreControl::pedirPistaDespegue() {
    return pedirPista(false);
}

int TorreControl::pedirPista(bool aterrizaje) {
    const auto inicio = std::chrono::steady_clock::now();
    std::unique_lock<std::mutex> lock(mutexPistas_);
    int& esperan = aterrizaje ? esperanAterrizar_ : esperanDespegar_;
    const int& esperanOtros = aterrizaje ? esperanDespegar_ : esperanAterrizar_;
    ++esperan;
    hayPista_.wait(lock, [&] {
        return (esperanOtros == 0 || turnoAterrizaje_ == aterrizaje) &&
               std::find(pistasLibres_.begin(), pistasLibres_.end(), true) != pistasLibres_.end();
    });
    const auto libre = std::find(pistasLibres_.begin(), pistasLibres_.end(), true);
    const int pista = static_cast<int>(libre - pistasLibres_.begin());
    *libre = false;
    --esperan;
    turnoAterrizaje_ = !aterrizaje;
    lock.unlock();
    hayPista_.notify_all();

    estadisticas_.registrarEspera(aterrizaje ? "avion (pista para aterrizar)" :
                                              "avion (pista para despegar)",
                                  std::chrono::steady_clock::now() - inicio);
    return pista;
}

void TorreControl::liberarPista(int pista) {
    {
        std::lock_guard<std::mutex> lock(mutexPistas_);
        pistasLibres_.at(pista) = true;
    }
    hayPista_.notify_all();
}

#include "TorreControl.h"

TorreControl::TorreControl(const Contexto& ctx, int pistas, int puertas, int aviones) {
    (void)ctx;
    (void)pistas;
    (void)puertas;
    (void)aviones;
}

int TorreControl::pedirPistaAterrizaje(int idAvion) {
    (void)idAvion;
    return 0;  // TODO (Persona 2)
}

int TorreControl::pedirPistaDespegue(int idAvion) {
    (void)idAvion;
    return 0;  // TODO (Persona 2)
}

void TorreControl::soltarPista(int pista, int idAvion) {
    (void)pista;
    (void)idAvion;
}

void TorreControl::pedirPuerta(int idAvion) {
    (void)idAvion;  // TODO (Persona 3)
}

void TorreControl::soltarPuerta(int idAvion) {
    (void)idAvion;
}

void TorreControl::actualizarEstado(int idAvion, const std::string& estado) {
    (void)idAvion;
    (void)estado;
}

void TorreControl::registrarOperacion() {
    // TODO (Persona 3)
}

long TorreControl::operaciones() {
    return 0;
}

void TorreControl::pantalla(int id) {
    (void)id;
    std::unique_lock<std::mutex> lock(_mPanel);
    while (!_panelCerrado) {
        _cvPanel.wait(lock);
    }
}

void TorreControl::operador() {
    std::unique_lock<std::mutex> lock(_mPanel);
    while (!_panelCerrado) {
        _cvPanel.wait(lock);
    }
}

void TorreControl::cerrarPanel() {
    std::lock_guard<std::mutex> lock(_mPanel);
    _panelCerrado = true;
    _cvPanel.notify_all();
}

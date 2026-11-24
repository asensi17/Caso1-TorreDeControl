
#include "ServiciosTierra.h"
#include <chrono>
#include <thread>
#include <stdexcept>

using namespace std;

// Constructor
ServiciosTierra::ServiciosTierra(
    int capacidad,
    Estadisticas& estadisticas,
    Vigilante& vigilante
)
    : capacidad(capacidad),
    estadisticas(estadisticas),
    vigilante(vigilante)
{
    if (capacidad <= 0) {
        throw invalid_argument("Capacidad incorrecta");
    }
}


// El avion solicita los tres servicios
void ServiciosTierra::encargarYEsperarServicios(int idAvion) {

    auto inicio = chrono::steady_clock::now();

    Servicio servicios[3] = {
        {idAvion, "combustible", 9},
        {idAvion, "limpieza", 3},
        {idAvion, "catering", 3}
    };

    {
        unique_lock<mutex> lock(mtx);

        pendientes[idAvion] = 3;

        for (int i = 0; i < 3; i++) {

            // Esperar si la cola esta llena
            hayEspacio.wait(lock, [this]() {
                return cerrada ||
                    static_cast<int>(cola.size()) < capacidad;
                });

            if (cerrada) {
                throw logic_error("Cola cerrada");
            }

            cola.push(servicios[i]);

            // Avisar a un operario
            hayTrabajo.notify_one();
        }

        // Esperar hasta que terminen los tres servicios
        serviciosTerminados.wait(lock, [this, idAvion]() {
            return pendientes.at(idAvion) == 0;
            });

        pendientes.erase(idAvion);
    }

    auto fin = chrono::steady_clock::now();

    estadisticas.registrarEspera(
        "avion (servicios)", fin - inicio
    );
}


// Funcion de cada operario
void ServiciosTierra::ejecutarOperario() {

    while (true) {

        Servicio servicio;

        {
            unique_lock<mutex> lock(mtx);

            // Esperar hasta que haya trabajo o se cierre la cola
            hayTrabajo.wait(lock, [this]() {
                return !cola.empty() || cerrada;
                });

            if (cerrada && cola.empty()) {
                return;
            }

            // Sacar el primer servicio de la cola
            servicio = cola.front();
            cola.pop();

            hayEspacio.notify_one();
        }

        // Realizar el servicio sin bloquear a los demas
        this_thread::sleep_for(
            chrono::milliseconds(servicio.duracionMs)
        );

        estadisticas.sumar("operaciones", 1);
        vigilante.progreso();

        {
            lock_guard<mutex> lock(mtx);

            pendientes.at(servicio.idAvion)--;

            // Si no quedan servicios, despertar al avion
            if (pendientes.at(servicio.idAvion) == 0) {
                serviciosTerminados.notify_all();
            }
        }
    }
}


// Cerrar la cola cuando terminen los aviones
void ServiciosTierra::cerrarCola() {

    {
        lock_guard<mutex> lock(mtx);
        cerrada = true;
    }

    hayTrabajo.notify_all();
    hayEspacio.notify_all();
}

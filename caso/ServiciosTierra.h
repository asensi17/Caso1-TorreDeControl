#pragma once // Evita que este archivo se incluya varias veces

#include <queue>              // Para guardar las peticiones en una cola
#include <vector>             // Para guardar los servicios pendientes
#include <mutex>              // Para utilizar el mutex (candado)
#include <condition_variable> // Para dormir y despertar hilos
#include <functional>         // Para llamar a una funcion del compañero
#include "Contexto.h"         // Herramientas que proporciona el profesor

class ServiciosTierra {

private:

    // Una peticion es una tarea que pide un avion.
    // Necesitamos saber que avion la pide
    // y que servicio necesita.
    struct Peticion {
        int idAvion;  // Numero identificador del avion
        int servicio; // 0 = combustible, 1 = limpieza, 2 = catering
    };

    // Cola donde guardamos las tareas que faltan por hacer.
    // La primera peticion que entra es la primera que sale.
    std::queue<Peticion> cola;

    // Guarda cuantos servicios le quedan a cada avion.
    // Por ejemplo, pendientes[2] guarda los servicios
    // que le quedan al avion numero 2.
    std::vector<int> pendientes;

    // Numero maximo de peticiones que caben en la cola.
    // Por defecto sera 6.
    int capacidad;

    // Indica si los servicios de tierra han cerrado.
    // Al principio estamos abiertos.
    bool cerrada = false;

    // Candado que impide que dos hilos modifiquen
    // los datos compartidos al mismo tiempo.
    std::mutex mtx;

    // Herramienta para dormir y despertar hilos.
    // wait() duerme un hilo.
    // notify_all() despierta a los que estan esperando.
    std::condition_variable aviso;

    // Contexto contiene las herramientas del profesor,
    // como Estadisticas y Vigilante.
    Contexto& ctx;

    // Funcion que utilizaremos para sumar una operacion
    // al contador general del aeropuerto.
    std::function<void()> incrementarOperaciones;

public:

    // Constructor: prepara los datos iniciales.
    ServiciosTierra(Contexto& contexto, int capacidadCola,
        int numAviones, std::function<void()> incrementar);

    // El avion pide sus 3 servicios y espera
    // hasta que los operarios los terminen.
    void solicitarYEsperarServicios(int idAvion);

    // Funcion que ejecutara cada trabajador.
    // Recoge peticiones de la cola y las realiza.
    void operario();

    // Cierra los servicios de tierra.
    void cerrar();

    // Devuelve true si el avion tiene todos sus servicios hechos.
    // Devuelve false si todavia le queda alguno.
    bool serviciosCompletados(int idAvion);
};
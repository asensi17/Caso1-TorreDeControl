#include "Semaforo.h"

// Guardamos el número inicial de permisos que nos pasan
Semaforo::Semaforo(int valorInicial) : _valor(valorInicial) {}

void Semaforo::acquire() {
    // Cerramos la puerta con unique_lock (lo usamos en vez de lock_guard porque wait()
    // necesita soltar el cerrojo mientras el hilo duerme y volver a cogerlo al despertar)
    std::unique_lock<std::mutex> lock(_m);

    // OJO: Usamos 'while' y NO 'if' porque si nos despiertan, otro hilo más rápido
    // nos puede quitar el sitio antes de que entremos, o el SO nos puede despertar sin querer.
    // Así que cada vez que abrimos los ojos, volvemos a mirar si de verdad queda sitio.
    while (_valor == 0) {
        // Suelta el cerrojo y se duerme de golpe hasta que alguien llame a release()
        _cv.wait(lock);
    }

    // Ya tenemos sitio seguro: nos gastamos un permiso
    _valor--;
    // Al llegar aquí se cierra la llave y unique_lock suelta el cerrojo solo
}

void Semaforo::release() {
    // Aquí sí vale lock_guard porque no nos vamos a dormir en mitad del método
    std::lock_guard<std::mutex> lock(_m);

    // Devolvemos el permiso (si nadie esperaba, queda apuntado para el siguiente que llegue)
    _valor++;

    // Pegamos un toque a uno de los hilos que estén dormidos en _cv para que entre
    _cv.notify_one();
}

bool Semaforo::tryAcquireFor(std::chrono::milliseconds plazo) {
    std::unique_lock<std::mutex> lock(_m);

    // Espera como máximo 'plazo' hasta que haya algún permiso (_valor > 0).
    // Fíjate que aquí la condición va al revés que en el while: le decimos cuándo PUEDE pasar.
    if (!_cv.wait_for(lock, plazo, [this]() { return _valor > 0; })) {
        // Si entra aquí es que se acabó el tiempo y sigue sin haber sitio
        return false;
    }

    // Si llega aquí es que consiguió permiso a tiempo: lo gastamos y avisamos que todo OK
    _valor--;
    return true;
}
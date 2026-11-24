// Prueba de Semaforo frente a std::counting_semaphore (apartado 1.4).

#include <atomic>
#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>
#include "Semaforo.h"

namespace {

    // Esta función prueba cualquier semáforo que tenga .acquire() y .release()
    // (así no copiamos y pegamos el mismo código dos veces)
    template <typename TipoSemaforo>
    bool probarLimiteTres(TipoSemaforo& sem) {
        std::atomic<int> dentro{ 0 };   // Hilos que están dentro ahora mismo
        std::atomic<bool> fallo{ false }; // Se pone a true si entran más de 3 a la vez

        std::vector<std::thread> hilos;
        for (int i = 0; i < 12; ++i) {
            hilos.emplace_back([&]() {
                for (int v = 0; v < 20; ++v) {
                    sem.acquire(); // Pide sitio (si no hay, espera dormido)

                    // Si al sumar 1 hay más de 3 dentro, algo va mal
                    if (++dentro > 3) {
                        fallo = true;
                    }

                    // Espera 300 microsegundos dentro del semáforo
                    std::this_thread::sleep_for(std::chrono::microseconds(300));

                    --dentro;      // Sale de la zona
                    sem.release(); // Devuelve el permiso
                }
                });
        }

        for (auto& h : hilos) {
            h.join(); // Espera a que acaben todos los hilos
        }

        return !fallo; // Devuelve true si nunca hubo más de 3
    }

} // namespace

int main() {
    // 1. Probamos nuestro Semaforo con valor inicial 3
    Semaforo mio(3);
    if (!probarLimiteTres(mio)) {
        std::cout << "FALLO en Semaforo (más de 3 hilos a la vez)\n";
        return 1;
    }

    // Si empieza en 0, tryAcquireFor(50ms) tiene que devolver false
    Semaforo mioCero(0);
    if (mioCero.tryAcquireFor(std::chrono::milliseconds(50))) {
        std::cout << "FALLO en tryAcquireFor con valor 0\n";
        return 1;
    }

    // 2. Probamos std::counting_semaphore<3> con valor inicial 3
    std::counting_semaphore<3> estandar(3);
    if (!probarLimiteTres(estandar)) {
        std::cout << "FALLO en counting_semaphore (más de 3 hilos a la vez)\n";
        return 1;
    }

    // Con valor 0, try_acquire_for(50ms) también tiene que devolver false
    std::counting_semaphore<3> estandarCero(0);
    if (estandarCero.try_acquire_for(std::chrono::milliseconds(50))) {
        std::cout << "FALLO en try_acquire_for con valor 0\n";
        return 1;
    }

    std::cout << "OK\n";
    return 0;
}
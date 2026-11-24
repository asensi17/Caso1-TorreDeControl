#include <iostream>
#include "ServiciosTierra.h"
#include "../semaforo/Semaforo.h"

void ServiciosTierra::limpiar() {
	std::unique_lock<std::mutex> lock();
	std::this_thread::sleep_for(std::chrono::seconds(3)); //Espera 3 segundos para la funcion limpiar antes de limpiarlo
	std::cout << "Limpiado" << std::endl;
};

void ServiciosTierra::repostar() {
    std::this_thread::sleep_for(std::chrono::milliseconds(9));
}

void ServiciosTierra::catering() {
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
}
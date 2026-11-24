# Proyecto base — Caso 1: Torre de Control (Temas 3 a 8)

Punto de partida obligatorio para los cinco casos. Cada carpeta genera una biblioteca o un ejecutable y tiene su propio `CMakeLists.txt`. El `CMakeLists.txt` de la raíz solo las reúne con `add_subdirectory`.

## Estructura del proyecto

| Carpeta | Genera | Qué contiene | ¿Se modifica? |
| :--- | :--- | :--- | :--- |
| `base/` | Biblioteca `base` | Clases auxiliares: `Config`, `Estadisticas`, `Traza`, `Vigilante`, `Lanzador` y la estructura `Contexto` | No |
| `semaforo/` | Biblioteca `semaforo` | Clase `Semaforo` (interfaz fijada en `Semaforo.h`; implementación en `Semaforo.cpp`) | Sí: `Semaforo.cpp` |
| `prueba_semaforo/` | Ejecutable `prueba_semaforo` | Prueba de `Semaforo` frente a `std::counting_semaphore` (apartado 1.4) | Sí: `main.cpp` |
| `caso/` | Ejecutable del caso | `main.cpp` y las 3 clases del Caso 1 (Torre de Control) | Sí: todo, incluido su `CMakeLists.txt` |
| `competicion/` | Ejecutable `competicion` | Programa de la competición (apartado 6) | Solo `Producto.cpp` |
| `ejemplo/` | Ejecutable `ejemplo` | Ejemplo pequeño que usa las clases auxiliares, con los modos `carrera` e `inanicion` | No, es para mirar |

---

## Primeros pasos

1. **Configurar `caso/CMakeLists.txt`:** poned el nombre del ejecutable de vuestro caso y, a medida que las creéis, añadid el `.h` y el `.cpp` de cada clase. Los `.h` también deben incluirse para que el IDE los muestre en el proyecto.
2. **Probar el ejemplo:** compilad y ejecutad `ejemplo` para ver cómo funcionan las clases auxiliares y los modos de fallo:
   ```bash
   ./ejemplo
   ./ejemplo --traza
   ./ejemplo --fallo carrera --estres 10
   ./ejemplo --fallo inanicion

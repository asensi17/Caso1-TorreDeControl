Registro de las consultas realizadas a la IA por cada miembro del grupo (apartado 1.3 del enunciado).

---

## Persona 5 (`kike`) — `Avion` y `main`

### Consulta 1 — Cómo provocar un interbloqueo en el avión

- **Fecha**: 24/11/2026
- **Miembro del equipo**: `kike` (Persona 5)
- **Herramienta usada**: Gemini
- **Prompt exacto**:
  > Estoy haciendo la clase Avion y tengo que meter el modo `--fallo interbloqueo`. En el caso normal pido primero la puerta y luego la pista para aterrizar. Qué tendría que cambiar en ese orden para que se produzca un interbloqueo sin que me des el código hecho?

- **Respuesta obtenida (parte relevante)**: Me explicó que, si todos los aviones piden primero la puerta y después la pista, no se genera una espera circular. Para provocar el interbloqueo, tendría que cambiar el orden y pedir primero la pista, aterrizar y, sin soltarla, pedir la puerta. De esta forma, si hay aviones esperando una pista para despegar mientras otros tienen la pista ocupada y esperan una puerta libre, pueden acabar bloqueándose entre ellos.

- **Qué se incorporó**: Añadí un `if (_interbloqueo)` en `Avion::ejecutar()` para que, en ese modo, el avión pida primero la pista, espere el tiempo del aterrizaje y después solicite la puerta sin liberar la pista.

- **Qué se descartó o corrigió y por qué**: Gemini también propuso añadir un `sleep` antes de solicitar la puerta para facilitar que se produjera el fallo, pero no fue necesario, ya que con los tiempos que teníamos el interbloqueo se producía igualmente.

---

### Consulta 2 — Por qué el `main` no terminaba al finalizar

- **Fecha**: 24/11/2026
- **Miembro del equipo**: `kike` (Persona 5)
- **Herramienta usada**: Gemini
- **Prompt exacto**:
  > En el main lanzo los hilos de aviones, operarios, pantallas y el operador, y al final les hago `join()` a todos. Los aviones terminan bien pero el programa no acaba nunca, porque puede ser?

- **Respuesta obtenida**: Me explicó que los hilos de los aviones sí terminan, pero los operarios, las pantallas y el operador siguen ejecutándose porque están en bucles esperando a que se cumpla una condición. Al hacer `join()` sin avisarles de que ya no van a llegar más aviones, se quedan esperando indefinidamente. Para solucionarlo, primero hay que esperar a que terminen los aviones, después avisar al resto de hilos mediante un método de cierre que active un booleano y llame a `notify_all()`, y finalmente esperar a que terminen esos otros hilos.

- **Qué se incorporó**: En `main.cpp` separé los `join()`. Primero espero a que terminen los aviones, después llamo a `servicios.cerrar()` y `torre.cerrarPanel()`, y por último hago `join()` de los operarios, las pantallas y el operador.

- **Qué se descartó o corrigió y por qué**: También se planteó usar `detach()` para los hilos que se ejecutan en segundo plano, pero lo descarté porque el enunciado pide que todos los hilos se cierren correctamente y de forma controlada.

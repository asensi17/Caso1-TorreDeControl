# Registro de uso de herramientas de IA (`IA.md`)

Registro de consultas a herramientas de inteligencia artificial conforme al apartado 1.3 del enunciado. Cada miembro del equipo documenta en su sección las consultas correspondientes a su parte del trabajo.

---

## Persona 5 (`kaik`) — Ciclo del Avión y Orquestación (`Avion` y `main`)

### Consulta 1 — Preparación del entorno Git y verificación del proyecto base
- **Fecha**: 23/11/2026
- **Miembro del equipo**: `kaik` (Persona 5)
- **Herramienta usada**: Google Antigravity
- **Prompt exacto**:
  > necesito que hagas la practica del enunciado.pdf , tengo el caso 4, preparame el environment de git porque aun no lo he hecho y vincula eso con el github "https://github.com/asensi17/Caso1-TorreDeControl" y luego te digo cual es mi parte del trabajo
- **Respuesta obtenida (parte relevante)**: Clonación del repositorio remoto en local vinculado a `origin` (`main`), configuración local de exclusiones de compilación en `.git/info/exclude`, compilación de verificación en modo `Release` con CMake + MSVC ejecutando `ejemplo.exe` correctamente y aviso sobre la diferencia entre el Caso 4 indicado en el prompt y el nombre del repositorio `Caso1-TorreDeControl`.
- **Qué se incorporó**: Configuración local del entorno de trabajo y verificación de compilación del proyecto base.
- **Qué se descartó o corrigió y por qué**: Se corrigió el caso a implementar tras revisar el documento de reparto del equipo, confirmando que el grupo desarrolla el **Caso 1 (Torre de control de un aeropuerto)** y no el Caso 4.
- **Ficheros y commits**: Ningún fichero ni commit en el repositorio (únicamente preparación local).

---

### Consulta 2 — Análisis de dependencias de la Persona 5 respecto al resto del equipo
- **Fecha**: 23/11/2026
- **Miembro del equipo**: `kaik` (Persona 5)
- **Herramienta usada**: Google Antigravity
- **Prompt exacto**:
  > he añadido en la carpeta el documento del plan de trabajo, yo soy la persona 5. Puedes hacer mi parte o dependes de la parte de alguno de los otros 5 miembros del grupo?
- **Respuesta obtenida (parte relevante)**: Identificación de las tareas de la Persona 5 (`caso/Avion.h`, `caso/Avion.cpp`, `caso/main.cpp` y `caso/CMakeLists.txt`). Análisis de dependencias: `Avion` y `main` dependen de las firmas públicas de `TorreControl` (Personas 2 y 3) y `ServiciosTierra` (Persona 4), pero no de `Semaforo` directamente (Persona 1). Propuesta de definir esqueletos mínimos (*stubs*) de `TorreControl` y `ServiciosTierra` para poder implementar, compilar y probar el cierre limpio desde el Hito 1 sin esperar al resto de módulos.
- **Qué se incorporó**: El planteamiento de contratos/esqueletos mínimos para desacoplar la implementación de `Avion` y `main`.
- **Qué se descartó o corrigió y por qué**: Se descartó programar la sincronización interna de `TorreControl` y `ServiciosTierra` para no invadir el trabajo asignado a las Personas 2, 3 y 4.
- **Ficheros y commits**: Diseño aplicado en el commit `666bb39`.

---

### Consulta 3 — Implementación de `Avion`, `main` y esqueletos de compilación
- **Fecha**: 24/11/2026
- **Miembro del equipo**: `kaik` (Persona 5)
- **Herramienta usada**: Google Antigravity
- **Prompt exacto**:
  > haz todo lo que puedas hacer que no dependa del código de otros miembros, y haz commit y pull. El código que sea muy sencillo, como el que aparece en el temario de la carpeta. Si vas a poner comentarios en el codigo que sean muy pocos y breves
- **Respuesta obtenida (parte relevante)**:
  - Implementación de `Avion` (`caso/Avion.h` y `caso/Avion.cpp`) con los 6 pasos del ciclo del avión (espera inicial `2i` ms, solicitud de puerta y pista, aterrizaje `2` ms, liberación de pista, encargo y espera de los 3 servicios de tierra, embarque `2` ms, comprobación de servicios pendientes, solicitud de pista de despegue, liberación de puerta, despegue `2` ms y liberación de pista) junto con el modo `--fallo interbloqueo` (pedir pista primero y puerta tras aterrizar sin soltar la pista).
  - Implementación de `caso/main.cpp` siguiendo la estructura de `ejemplo/main.cpp`: lectura de las 6 claves con `Config` (`aviones`, `pistas`, `puertas`, `cola`, `operarios`, `pantallas`), creación de `Vigilante`, `Contexto`, `TorreControl`, `ServiciosTierra` y `Avion`, lanzamiento de los 4 tipos de hilos, secuencia de cierre ordenado (`join` de aviones, cierre de `ServiciosTierra` y del panel de `TorreControl`, `join` del resto de hilos), validación del invariante final (`operaciones == 5 * aviones`) y llamada a `Lanzador`.
  - Configuración del ejecutable `aeropuerto` en `caso/CMakeLists.txt` y creación de esqueletos mínimos en `caso/TorreControl.h/.cpp` y `caso/ServiciosTierra.h/.cpp` con parada limpia mediante `std::mutex` y `std::condition_variable` (RAII, sin espera activa).
- **Qué se incorporó**: Los ficheros completos de la Persona 5 (`Avion.h`, `Avion.cpp`, `main.cpp`, `caso/CMakeLists.txt`) y los esqueletos temporales de `TorreControl` y `ServiciosTierra` para permitir la compilación limpia con `/W4` (`-Wall -Wextra -Wpedantic`).
- **Qué se descartó o corrigió y por qué**: Los métodos de pistas, puertas, panel, contador de operaciones y cola de servicios se dejaron como *stubs* mínimos (`TODO`) porque su implementación corresponde a las Personas 2, 3 y 4; al integrarse después cambios remotos en `main`, el commit local inicial (`33ff6c5`) se rebasó limpiamente dando lugar al commit `666bb39`.
- **Ficheros y commits**: `caso/Avion.h`, `caso/Avion.cpp`, `caso/main.cpp`, `caso/CMakeLists.txt`, `caso/TorreControl.h`, `caso/TorreControl.cpp`, `caso/ServiciosTierra.h`, `caso/ServiciosTierra.cpp` — commit `666bb39` (*Implementar Avion, main y esqueletos de TorreControl y ServiciosTierra*).

---

### Consulta 4 — Redacción del registro `IA.md` para la Persona 5
- **Fecha**: 24/11/2026
- **Miembro del equipo**: `kaik` (Persona 5)
- **Herramienta usada**: Google Antigravity
- **Prompt exacto**:
  > puedes generar el archivo IA.md (solo de lo que corresponde a mi parte del trabajo) y hacer commit + push?
- **Respuesta obtenida (parte relevante)**: Creación del documento `IA.md` en la raíz del repositorio recogiendo fecha, miembro, herramienta, prompts exactos, respuestas relevantes, código incorporado, descartes/correcciones y trazabilidad a ficheros y commits de la Persona 5.
- **Qué se incorporó**: Este fichero `IA.md` con la sección correspondiente a la Persona 5.
- **Qué se descartó o corrigió y por qué**: Se documentó exclusivamente el trabajo de la Persona 5 para que el resto de integrantes añadan sus propias entradas en sus respectivos commits.
- **Ficheros y commits**: `IA.md` (incorporado en el commit de creación de `IA.md`).

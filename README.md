# Agente Inteligente para 5-en-Raya Táctico (Modo Competición 9x9) en C++

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Build System](https://img.shields.io/badge/Build-CMake-064F8C.svg)](https://cmake.org/)
[![Graphics Engine](https://img.shields.io/badge/Graphics-OpenGL%20%2F%20FreeGLUT-orange.svg)](https://www.opengl.org/)
[![Status: 8/8 Wins](https://img.shields.io/badge/Tournament-8%2F8%20Victories-brightgreen.svg)](#resultados-y-rendimiento-competitivo)

Implementación de algoritmos de búsqueda con adversario (Status, MiniMax y Poda Alfa-Beta) junto con el diseño evolutivo de funciones de evaluación heurística para competir en una variante avanzada de 5-en-Raya sobre tablero de 9x9 con reglas asimétricas.

---

## Contexto Académico

Proyecto desarrollado durante el **segundo curso del Grado en Ingeniería Informática** en la **Universidad de Granada (UGR)** para la asignatura *Inteligencia Artificial* (curso 2025/2026), impartida en la **E.T.S. de Ingenierías Informática y de Telecomunicación (ETSIIT)**.

* **Entorno base:** El simulador gráfico (interfaz en OpenGL), el árbitro de reglas del juego y la infraestructura de ejecución con y sin hebras fueron suministrados por el **Departamento de Ciencias de la Computación e Inteligencia Artificial (DECSAI / CCIA)** de la UGR.
* **Autoría del trabajo propio:** Diseño e implementación íntegra de la toma de decisiones del agente dentro de los archivos `AgenteEstudiante.hpp` y `AgenteEstudiante.cpp` (búsqueda exhaustiva, exploración de árboles de juego, podas y funciones heurísticas).

---

## Reglas del Modo Competición (9x9)

A diferencia del juego de alineación clásico, el modo competición añade restricciones que alteran profundamente el espacio de estados y la combinatoria de juego:

* **Configuración inicial:** Tablero de $9 \times 9$ con un diamante inicial de 4 fichas pre-colocadas en el centro (2 de cada jugador) para forzar la conectividad inmediata.
* **Secuencia de turnos asimétricos (1-2-2-2):** El Jugador 1 comienza colocando 1 sola pieza; en los turnos sucesivos, los jugadores colocan hasta 2 piezas consecutivas por turno.
* **Regla de la Trinidad (Módulo 3):** Solo se permite jugar en una celda $(f, c)$ si $(f + c) \pmod 3 == \text{faseActual} \pmod 3$, avanzando la fase cuando el jugador cede el turno.
* **Regla de Adyacencia:** Toda nueva pieza debe colocarse obligatoriamente en una celda libre adyacente (en cualquiera de las 8 direcciones) a una pieza ya existente.
* **Casillas Especiales:**
  * **Verdes (Místicas):** Otorgan $+1$ movimiento extra en el turno actual.
  * **Rojas (Sabotaje):** Convierten automáticamente la ficha colocada en una pieza del rival.
  * **Amarillas (Bomba):** Detonan y limpian por completo la fila y columna asociadas, conservando únicamente la casilla detonada.

---

## Algoritmos de Búsqueda Implementados

* **Búsqueda Exhaustiva (`Status`):** Exploración completa del árbol de juego para determinar veredictos teóricos exactos (Victoria, Derrota o Empate) en subárboles pequeños o finales de partida, incorporando corte temprano ante líneas ganadoras forzadas.
* **Algoritmo MiniMax:** Exploración recursiva alternando capas de maximización y minimización con control de profundidad. Integra comprobación de temporizador (`std::chrono`) para interrumpir la búsqueda de forma segura si se alcanza el tiempo límite por jugada.
* **Poda Alfa-Beta (`alfaBeta`):** Optimización sobre MiniMax mediante actualización de cotas $(\alpha, \beta)$ para descartar ramas irrelevantes, permitiendo alcanzar hasta profundidad 7 en el tiempo asignado de competición.

---

## Evolución y Diseño de Heurísticas

El desarrollo del evaluador estático siguió un proceso iterativo de ensayo y error hasta dar con una función capaz de contrarrestar el doble turno rival:

* **Heurística 1 (`heuristica1` - Versión de Competición):**
  * **Ponderación asimétrica ultra-defensiva:** Para neutralizar los turnos dobles (1-2-2-2), penaliza las amenazas del oponente (líneas de 3 y 4 fichas) hasta **4 veces más** de lo que premia las propias ($2 \cdot 10^6$ vs $5 \cdot 10^5$), obligando a la IA a taponar ataques antes de expandir.
  * **Multiplicadores posicionales:** Aplica un factor de $\times 2.5$ a las casillas verdes para priorizar turnos extra y $\times 1.5$ a las amarillas para controlar zonas de detonación, ponderadas según la cercanía al centro del tablero.
* **Heurística 2 (`heuristica2` - Test de Pesos):**
  * Introdujo una escala no lineal de alineaciones (las líneas de 4 valen mucho más que las de 3 y 2), pero con pesos simétricos ataque/defensa y sin lectura de celdas especiales.
  * *Punto débil:* Al no priorizar la defensa frente a los turnos dobles ni considerar las casillas verdes, los rivales avanzados encadenaban victorias rápidas.
* **Heurística 3 (`heuristica3` - Prototipo Inicial):**
  * Control de proximidad al centro junto a bonificaciones/penalizaciones fijas $(+10 / -10)$ en casillas verdes y rojas.
  * *Punto débil:* Contaba todas las combinaciones con peso idéntico (100 puntos), sin distinguir entre una amenaza crítica de 4 fichas y una línea testimonial de 2.

---

## Resultados y Rendimiento Competitivo

Evaluación frente a los cuatro agentes Ninja oficiales del entorno (tanto abriendo como Jugador 1 como respondiendo como Jugador 2):

### Enfrentamiento contra Agentes Ninja
| Heurística | Ninja 1 (J1 / J2) | Ninja 2 (J1 / J2) | Ninja 3 (J1 / J2) | Ninja 4 (J1 / J2) | Balance Total |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Heurística 1 (Final)** | **Victoria / Victoria** | **Victoria / Victoria** | **Victoria / Victoria** | **Victoria / Victoria** | **8 / 8 Victorias (100%)** |
| **Heurística 2** | Victoria / Victoria | Victoria / Victoria | Derrota / Victoria | Victoria / Derrota | 6 / 8 Victoria |
| **Heurística 3** | Victoria / Victoria | Victoria / Victoria | Victoria / Victoria | Derrota / Victoria | 7 / 8 Victorias |

### Duelos Cruzados entre Heurísticas
* **H1 vs H2:** Victoria contundente de **Heurística 1** en ambos roles (J1 y J2).
* **H1 vs H3:** Victoria de **Heurística 1** en ambos roles (J1 y J2).
* **H2 vs H3:** Victoria de **Heurística 2** en ambos roles debido al escalado exponencial de líneas.

---

## Estructura del Código Propio

```text
Comportamiento_Agente/
├── AgenteEstudiante.hpp  # Declaración de la clase AgenteEstudiante, constantes y selectores
└── AgenteEstudiante.cpp  # Implementación de Status, Minimax, Alfa-Beta y las funciones heurísticas (0-3)
```

---

## Compilación y Ejecución

### Requisitos previos (Linux / Ubuntu)
```bash
sudo apt update
sudo apt install build-essential cmake freeglut3-dev libjpeg-dev libxmu-dev libxi-dev
```

### Compilación
```bash
# Primera compilación e instalación de dependencias
./install.sh

# Recompilaciones rápidas
make -j$(nproc)
```

### Modos de Ejecución

* **Humano vs Agente Inteligente (Poda Alfa-Beta en GUI):**
  ```bash
  ./n_en_raya -p1 humano -p2 inteligente -f 9 -c 9 -n 5
  ```

* **Duelo de Competición (Agente Inteligente vs Ninja 2):**
  ```bash
  ./n_en_raya -p1 inteligente -id1 1 -p2 ninja2 -f 9 -c 9 -n 5
  ```

* **Benchmarking sin interfaz gráfica (`-nogui`):**
  ```bash
  # Enfrentamiento entre Heurística 1 y Heurística 2 a profundidad 4
  ./n_en_raya -p1 inteligente -id1 1 -p2 inteligente -id2 2 -d 4 -nogui
  ```

* **Resolución Teórica con Status (en tableros reducidos):**
  ```bash
  ./n_en_raya -p1 status -f 3 -c 3 -n 3 -nogui
  ```

---

## Créditos 

* **Desarrollo del Agente:** Almudena O. ([@almuocon](https://github.com/almuocon)) — Estudiante de Ingeniería Informática en la **Universidad de Granada (ETSIIT - UGR)**.
* **Entorno y Motor del Juego:** Departamento de Ciencias de la Computación e Inteligencia Artificial (**DECSAI / CCIA**), Universidad de Granada.

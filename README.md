Agente Inteligente para 5-en-Raya Táctico (Modo Competición 9x9) en C++

Proyecto académico desarrollado para la asignatura Inteligencia Artificial (curso 2025/2026), E.T.S. de Ingenierías Informática y de Telecomunicación (ETSIIT), Universidad de Granada.

El proyecto implementa un agente autónomo competitivo capaz de jugar a una versión avanzada del clásico juego de alineación sobre un tablero de 9x9 con restricciones matemáticas complejas (Regla de la Trinidad, turnos asimétricos 1-2-2-2, adyacencia y casillas especiales con efectos de sabotaje, bombas y turnos extra).

Créditos y Autoría del Software Base:

Simulador e Interfaz Gráfica: El entorno gráfico basado en OpenGL, el motor de reglas del juego y la arquitectura general pertenecen al Departamento de Ciencias de la Computación e Inteligencia Artificial (CCIA) de la Universidad de Granada.

Mi Contribución: El diseño algorítmico e implementación completa de la inteligencia del agente dentro del directorio Comportamiento_Agente (archivos AgenteEstudiante.cpp y AgenteEstudiante.hpp), incluyendo la exploración de árboles de juego y las funciones de evaluación heurística.

Algoritmos Implementados:

Búsqueda Exhaustiva (Status): Algoritmo para la resolución exacta y determinación del veredicto teórico (Victoria, Derrota, Empate) en subárboles de juego.

Algoritmo MiniMax: Exploración completa del árbol de decisiones alternando capas maximizadoras y minimizadoras.

Poda Alfa-Beta: Optimización del algoritmo Minimax para podar ramas irrelevantes del árbol de búsqueda, permitiendo alcanzar mayores profundidades de exploración en tiempo real.

Función Heurística Competitiva: Evaluación ponderada de alineaciones potenciales (líneas de 3, 4 y 5 fichas), control del centro y casillas adyacentes estratégicas, y gestión táctica de celdas especiales (aprovechamiento de turnos extra en casillas verdes y evasión de trampas o bombas).

Tecnologías Utilizadas:

Lenguaje: C++ (C++11/17)

Técnicas de IA: Búsqueda con adversario, teoría de juegos, poda Alfa-Beta, heurísticas de evaluación estática.

Compilación y dependencias: CMake, Make, FreeGLUT / OpenGL.

Compilación y Ejecución (Linux):

Compilación:
./install.sh
(O para recompilaciones: make -j)

Partida Humano vs Agente Inteligente (Alfa-Beta):
./n_en_raya -p1 humano -p2 inteligente -f 9 -c 9 -n 5

Agente Inteligente vs Agente Ninja de prueba:
./n_en_raya -p1 inteligente -id1 1 -p2 ninja2 -f 9 -c 9 -n 5

Modo Benchmarking (Sin interfaz gráfica, para pruebas de rendimiento):
./n_en_raya -p1 inteligente -id1 1 -p2 inteligente -id2 1 -d 4 -t 10.0 -nogui

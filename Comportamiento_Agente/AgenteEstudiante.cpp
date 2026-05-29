#include "AgenteEstudiante.hpp"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath> 
#include <functional>

AgenteEstudiante::AgenteEstudiante(int id, int profundidadMax, double tiempoMax, int numHeuristica, ModoJuego modo) 
    : id(id), profundidadMax(profundidadMax), tiempoMaxSegundos(tiempoMax), numHeuristica(numHeuristica), modo(modo), abortarBanda(false) {
    nodosVisitados = 0;
}

bool AgenteEstudiante::tieneLimiteDeTiempo() const {
    return modo != ModoJuego::STATUS;
}

std::pair<int, int> AgenteEstudiante::think(const Tablero& tablero) {
    std::pair<int, int> mejor;
    nodosVisitados = 0;
    abortarBanda = false;
    inicioBusqueda = std::chrono::steady_clock::now();

    switch (modo)
    {
    case ModoJuego::ALEATORIO:
        return JuegaAleatorio(tablero);
        break;
    
    case ModoJuego::STATUS:
        Status(tablero, mejor);
        return mejor;
        break;    

    case ModoJuego::MINIMAX:
        minimax(tablero, 0, profundidadMax, mejor);
        return mejor;
        break; 

    case ModoJuego::INTELIGENTE:
        return JuegaInteligente(tablero);   
        break;
    }
        
    return {-1, -1};
}


/**
 * @brief Compara dos tableros para identificar cuál ha sido el movimiento realizado.
 * @param padre Estado inicial del tablero.
 * @param hijo Estado resultante tras un movimiento.
 * @return Un par (fila, columna) con la posición de la nueva pieza.
 */
std::pair<int, int> SacarMovimiento(const Tablero& padre, const Tablero &hijo){
    for(int f=0; f<padre.getFilas(); ++f)
        for(int c=0; c<padre.getColumnas(); ++c)
            if (padre.getCelda(f,c) == 0 && hijo.getCelda(f,c) != 0) 
                return {f, c};
    return {-1, -1};
}

/**
 * @brief Implementa un agente que juega de forma totalmente aleatoria.
 * @param tablero Estado actual del juego.
 * @return La jugada elegida al azar.
 */
std::pair<int, int> AgenteEstudiante::JuegaAleatorio(const Tablero& tablero) {

    // Calculo los tableros descendientes de tablero
    auto sucesores = tablero.getSucesores();

    // Si no tiene descendientes, paso el turno
    if (sucesores.empty()) return {-1, -1};

    // Elijo aleatoriamente uno de los descendientes
    int elegido = rand() % sucesores.size();

    // Saco el movimiento realizado comparando el tablero original con el elegido.
    std::pair<int,int> Mov = SacarMovimiento(tablero, sucesores[elegido]);

    return Mov;
}


/**
 * @brief Algoritmo de resolución completa para estados de final de juego.
 * Determina si una posición está matemáticamente ganada, perdida o empatada.
 * @param tablero Estado a evaluar.
 * @param Mov [Salida] La jugada óptima encontrada.
 * @return Resultado del análisis (VICTORIA, DERROTA o EMPATE).
 */
AgenteEstudiante::Resultado AgenteEstudiante::Status(const Tablero &tablero, std::pair<int,int> &Mov) {
    /* ============== Este trozo de código se tiene que quedar aquí  =============== */
    nodosVisitados++;
    /* ============== Empieza a partir de aquí tu implementación  =============== */
    //Condicion de finalizacion
    int idMaquina = 1;
    if (id == 1){
        idMaquina = 2;
    }
    int ganador = tablero.comprobarGanador();

    if(ganador == id){
        return Resultado::VICTORIA;
    }else if(ganador == idMaquina){
        return Resultado::DERROTA;
    }else if(ganador == -1){
        return Resultado::EMPATE;
    }

    //hijos del tablero
    std::vector<Tablero> hijos = tablero.getSucesores();
    if (hijos.empty()) {
        return Resultado::EMPATE;
    }

    //jugada (recursividad)
    int turnoActual = tablero.getJugadorTurno();
    if(turnoActual == id){ //mi turno
        Resultado mejorResultado = Resultado::DERROTA;
        
        for (const Tablero &h : hijos){
            std::pair<int,int> movHijo;
            Resultado r = Status(h, movHijo);

            //camino que asegura ganar
            if( r == Resultado::VICTORIA){
                Mov = SacarMovimiento(tablero, h);
                return Resultado::VICTORIA;
            }

            //empatar
            if( r == Resultado::EMPATE && mejorResultado == Resultado::DERROTA){
                mejorResultado = Resultado::EMPATE;
                Mov = SacarMovimiento(tablero, h);
            }
        }
        if(mejorResultado == Resultado::DERROTA){
            Mov = SacarMovimiento(tablero, hijos[0]);
        }

        return mejorResultado;

    }else{ //turno del rival
        Resultado peorResultado = Resultado::VICTORIA;

        for(const Tablero &h : hijos){
            std::pair<int, int> movHijo;
            Resultado r = Status(h, movHijo);

            if(r == Resultado::DERROTA){
                return Resultado::DERROTA;
            }

            if (r == Resultado::EMPATE && peorResultado == Resultado::VICTORIA) {
                peorResultado = Resultado::EMPATE;
            } 
        }
        return peorResultado;
    }
    
}



/**
 * @brief Implementación del algoritmo Minimax clásico.
 * @param tablero Estado actual.
 * @param profundidad Nivel actual en el árbol de búsqueda.
 * @param prof_Max Límite de profundidad de la búsqueda.
 * @param Mov [Salida] La mejor jugada encontrada en la raíz.
 * @return Valor heurístico del estado.
 */
double AgenteEstudiante::minimax(const Tablero &tablero, int profundidad, int prof_Max, std::pair<int,int> &Mov) {
    /* ============== Este trozo de código se tiene que quedar aquí  =============== */
    nodosVisitados++;
    if (abortarBanda) return 0;
    
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() - inicioBusqueda).count() > tiempoMaxSegundos) {
        abortarBanda = true;
        return 0;
    }
    /* ============== Empieza a partir de aquí tu implementación  =============== */
    //Condicion de finalizacion
    int idMaquina = 1;
    if (id == 1){
        idMaquina = 2;
    }
    int ganador = tablero.comprobarGanador();

    if(ganador == id){
        return GANAR;
    }else if(ganador == idMaquina){
        return PERDER;
    }else if(ganador == -1){
        return 0;
    }

    if (profundidad == prof_Max) {
        return heuristica(tablero);
    }

    //hijos
    std::vector<Tablero> hijos = tablero.getSucesores();
    if (hijos.empty()) return 0;
    int turnoActual = tablero.getJugadorTurno();

    //recursividad
    if (turnoActual == id) {
        double mejorValor = MenosInfinito;
        if(profundidad == 0){
            Mov = SacarMovimiento(tablero, hijos [0]);
        }

        for (const Tablero &h : hijos){
            std::pair<int,int> movHijo;
            double valorHijo = minimax(h, profundidad + 1, prof_Max, movHijo);

            if(valorHijo > mejorValor){
                mejorValor = valorHijo;
                if (profundidad == 0) {
                    Mov = SacarMovimiento(tablero, h);
                }
            }
        }
        return mejorValor;

    } else {
        double peorValor = MasInfinito; 

        for (const Tablero &h : hijos) {
            std::pair<int,int> movHijo;
            double valorHijo = minimax(h, profundidad + 1, prof_Max, movHijo);

            if (valorHijo < peorValor) {
                peorValor = valorHijo;
            }
        }
        return peorValor;
    }
}


/**
 * @brief Punto de entrada para el juego inteligente.
 * @param tablero Estado actual del juego.
 * @return La jugada elegida por el algoritmo de búsqueda.
 */
std::pair<int, int> AgenteEstudiante::JuegaInteligente(const Tablero& tablero) {
    std::pair<int,int> Mov;

    double valor = alfaBeta(tablero, 0, profundidadMax, MenosInfinito, MasInfinito, Mov);
    std::cout << "Valor Minimax: " << valor << "\tJugada: (" << Mov.first << ", " << Mov.second << ")\n";
    return Mov;
}




/**
 * @brief Implementación del algoritmo Minimax con Poda Alfa-Beta.
 * @param tablero Estado actual.
 * @param profundidad Nivel actual en el árbol de búsqueda.
 * @param prof_Max Límite de profundidad de la búsqueda.
 * @param alfa Valor mínimo garantizado para el jugador MAX.
 * @param beta Valor máximo garantizado para el jugador MIN.
 * @param Mov [Salida] La mejor jugada encontrada en la raíz.
 * @return Valor heurístico del estado tras la poda.
 */
double AgenteEstudiante::alfaBeta(const Tablero &tablero, int profundidad, int prof_Max, double alfa, double beta, std::pair<int,int> &Mov) {
    /* ============== Este trozo de código se tiene que quedar aquí  =============== */
    nodosVisitados++;
    if (abortarBanda) return 0;
    
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() - inicioBusqueda).count() > tiempoMaxSegundos) {
        abortarBanda = true;
        return 0;
    }
    /* ============== Empieza a partir de aquí tu implementación  =============== */
    //Condicion de finalizacion
    int idMaquina = 1;
    if (id == 1){
        idMaquina = 2;
    }
    int ganador = tablero.comprobarGanador();

    if(ganador == id){
        return GANAR;
    }else if(ganador == idMaquina){
        return PERDER;
    }else if(ganador == -1){
        return 0;
    }

    if (profundidad == prof_Max) {
        return heuristica(tablero);
    }

    //hijos
    std::vector<Tablero> hijos = tablero.getSucesores();
    if (hijos.empty()) return 0;
    int turnoActual = tablero.getJugadorTurno();

    //recursividad
    if (turnoActual == id) {
        double mejorValor = MenosInfinito;
        if(profundidad == 0){
            Mov = SacarMovimiento(tablero, hijos [0]);
        }

        for (const Tablero &h : hijos){
            std::pair<int,int> movHijo;
            double valorHijo = alfaBeta(h, profundidad + 1, prof_Max, alfa, beta, movHijo);

            if(valorHijo > mejorValor){
                mejorValor = valorHijo;
                if (profundidad == 0) {
                    Mov = SacarMovimiento(tablero, h);
                }
            }
            if (mejorValor >= beta) {
                break; 
            }
            if (mejorValor > alfa) {
                alfa = mejorValor;
            }
        }
        return mejorValor;

    } else {
        double peorValor = MasInfinito; 

        for (const Tablero &h : hijos) {
            std::pair<int,int> movHijo;
            double valorHijo = alfaBeta(h, profundidad + 1, prof_Max, alfa, beta, movHijo);

            if (valorHijo < peorValor) {
                peorValor = valorHijo;
            }
            if (peorValor <= alfa) {
                break; 
            }
            if (peorValor < beta) {
                beta = peorValor;
            }
        }
        return peorValor;
    }
}

/**
 * @brief Función heurística para evaluar la calidad de un tablero.
 * @param tablero Estado a evaluar.
 * @return Puntuación numérica (positiva para ventaja de J1, negativa para J2).
 */
double AgenteEstudiante::heuristica(const Tablero& tablero) {
    switch(numHeuristica) {
        case 0: return heuristicaPrueba(tablero);
                break;
        case 1: return heuristica1(tablero);
                break;
        case 2: return heuristica2(tablero);
                break;
        case 3: return heuristica3(tablero);
                break;
        default: return heuristica1(tablero);
    }
}

double AgenteEstudiante::heuristicaPrueba(const Tablero& tablero) {
    // n es el número de fichas en línea para ganar.
    int n = tablero.getNParaGanar();
    int oponente = (id == 1) ? 2 : 1;
    double score_positivo = 0;

    double score_negativo = 0;

    for (int f=0; f< tablero.getFilas(); f++ ){
        for (int c = 0; c< tablero.getColumnas(); c++){
            if (tablero.getCelda(f,c) != 0 ){
                int valor = tablero.getFilas()-abs(f-(tablero.getFilas()/2)) + tablero.getColumnas()-abs(c-(tablero.getColumnas()/2)); 
                if (tablero.getCelda(f,c) == id){
                  score_positivo += valor;
                 }
                else {
                  score_negativo += valor;
                }
            }
        }
    }

   
    return score_positivo - score_negativo;
}


double AgenteEstudiante::heuristica1(const Tablero& tablero) {
    int oponente = (id == 1) ? 2 : 1;
    double score_positivo = 0;
    double score_negativo = 0;

    int filas = tablero.getFilas();
    int columnas = tablero.getColumnas();
    int centroF = filas / 2;
    int centroC = columnas / 2;

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (tablero.getCelda(f, c) != 0) {
                double valor = filas - abs(f - centroF) + columnas - abs(c - centroC);
                
                Tablero::TipoCelda tipo = tablero.getTipoCelda(f, c);
                if (tipo == Tablero::TipoCelda::VERDE) {
                    valor *= 2.5; 
                } else if (tipo == Tablero::TipoCelda::AMARILLO) {
                    valor *= 1.5;
                }

                if (tablero.getCelda(f, c) == id) {
                    score_positivo += valor;
                } else {
                    score_negativo += valor;
                }
            }
        }
    }

    double mi_4 = tablero.contarCombinaciones(4, id);
    double mi_3 = tablero.contarCombinaciones(3, id);
    double mi_2 = tablero.contarCombinaciones(2, id);

    double op_4 = tablero.contarCombinaciones(4, oponente);
    double op_3 = tablero.contarCombinaciones(3, oponente);
    double op_2 = tablero.contarCombinaciones(2, oponente);

    double score_lineas = (mi_4 * 500000.0 + mi_3 * 5000.0 + mi_2 * 50.0)
                        - (op_4 * 2000000.0 + op_3 * 40000.0 + op_2 * 200.0);

    return (score_positivo - score_negativo) + score_lineas;
}

double AgenteEstudiante::heuristica2(const Tablero& tablero) {
    //A implementar por el estudiante
return 0;
}

double AgenteEstudiante::heuristica3(const Tablero& tablero) {
    //A implementar por el estudiante
return 0;
}

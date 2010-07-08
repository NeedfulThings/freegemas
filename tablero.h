#ifndef _TABLERO_H_
#define _TABLERO_H_

#include <Gosu/Gosu.hpp>

#include <vector>
#include <tr1/array>
using namespace std;

#include "log.h"

enum tCasilla { casVacia , casBlanca , casRoja, casLila, casNaranja, casVerde, casAmarilla, casAzul };

/**
 * @class Casilla
 *
 * @brief Representa una casilla del tablero.
 *
 * Además de la gema, tiene atributos para la gestión de las animaciones 
 * cuando desaparecen otras gemas.
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


struct Casilla{
    /// Tipo de gema que contiene la casilla.
    tCasilla tipo;

    Casilla(const tCasilla & t = casVacia) : origY(0), destY(0), debeCaer(false){
	tipo = t;
    }

    bool operator==(const Casilla & C){
	return C.tipo == tipo;
    }

    bool operator==(const tCasilla & t){
	return tipo == t;
    }

    operator tCasilla(){ return tipo; }

    /// Posición inicial vertical de la casilla - Se usa para hacer las animaciones
    int origY;

    /** Desplazamiento vertical. 

	Cuando se ha hecho un grupo de figuras, y éstas desaparecen,
	las que están encima deben caer. Este atributo cuenta el
	número de casillas que debe caer hacia abajo.
    */

    int destY; 
    
    /// Indica si debe o no caer
    bool debeCaer; 
};

/**
 * @class Punto cartesiano con coordenadas x e y
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


struct coord{
    int x, y;
    coord(int x = 0, int y = 0) : x(x), y(y) { }
    bool operator ==(const coord & c){
	return (c.x == x && c.y == y);
    }
};

/**
 * @class Tablero
 *
 * @brief Representa un tablero de juego en un momento dado.
 *
 * Contiene una matriz de 8x8 con el contenido de las casillas
 * además de algoritmos que permiten hacer comprobaciones sobre el tablero.
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


class Tablero{
public:
    Tablero();
    ~Tablero();

    /// Intercambia el valor de la casilla x1,y1 por el de x2,y2
    void swap(int x1, int y1, int x2, int y2);

    /// Vacía la casilla x,y
    void del(int x, int y);
    
    /// Genera un tablero nuevo completamente aleatorio
    void generar();

    /** Calcula las posiciones de las casillas tras borrar otras casillas. 

	Cuando se hace un grupo de tres o más casillas, esta función
	actualiza las casillas que estuvieran por encima para asignarles
	una nueva posición.
    */

    void calcularMovimientosCaida();


    /// Aplica los cambios calculados en la función calcularMovimientosCaida
    void aplicarCaida();

    /// Rellena los espacios vacíos con nuevas casillas aleatorias
    void rellenarEspacios();

    /**
     * @brief Comprueba si hay algún grupo de tres o más casillas en horizontal o vertical
     *
     * @return Conjunto de coordenadas con las casillas que forman parte de un grupo
     *
     */

    vector<coord> comprobar();

    /// Comprueba si existen movimientos con los que hacer algún grupo en el tablero actual
    bool existeSolucion();

    /// Matriz de casillas
    tr1::array< tr1::array<Casilla, 8>, 8> casillas;
};
#endif /* _TABLERO_H_ */

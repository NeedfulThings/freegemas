#ifndef _BOARD_H_
#define _BOARD_H_

#include <Gosu/Gosu.hpp>

#include <boost/foreach.hpp>

#include <algorithm>
#include <vector>
#include <tr1/array>
using namespace std;

#include "log.h"

enum tSquare { sqEmpty, sqWhite, sqRed, sqPurple, sqOrange, sqGreen, sqYellow, sqBlue };

/**
 * @class Casilla
 *
 * @brief Representa una casilla del board.
 *
 * Además de la gema, tiene atributos para la gestión de las animaciones 
 * cuando desaparecen otras gemas.
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


struct Square{
    /// Tipo de gema que contiene la casilla.
    tSquare tipo;

    Square(const tSquare & t = sqEmpty) : origY(0), destY(0), mustFall(false){
	tipo = t;
    }

    bool operator==(const Square & C){
	return C.tipo == tipo;
    }

    bool operator==(const tSquare & t){
	return tipo == t;
    }

    operator tSquare(){ return tipo; }

    /// Posición inicial vertical de la casilla - Se usa para hacer las animaciones
    int origY;

    /** Desplazamiento vertical. 

	Cuando se ha hecho un grupo de figuras, y éstas desaparecen,
	las que están encima deben caer. Este atributo cuenta el
	número de casillas que debe caer hacia abajo.
    */

    int destY; 
    
    /// Indica si debe o no caer
    bool mustFall; 
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
	return (c.x == x && 
		c.y == y);
    }

    friend ostream& operator << (ostream& out, coord & C){
	out << "(" << C.x << "," << C.y << ")";

	return out;
    }
};

class Match{
public:
    void push_back(const coord & c){
	matchedSquares.push_back(c);
    }

    coord midSquare() const{
	return matchedSquares[ matchedSquares.size() / 2];
    }

    int numSquares() const{
	return matchedSquares.size();
    }

    bool find(coord c) {
	return (std::find(matchedSquares.begin(), matchedSquares.end(), c) 
		!= matchedSquares.end());
    }
    int size() const{
	return matchedSquares.size();
    }
    coord operator[](size_t n) const{
	return matchedSquares[n];
    }

    friend ostream& operator << (ostream& out, Match & M){
	out << "Match (" << M.matchedSquares.size() << "): ";
	for (size_t i = 0; i < M.matchedSquares.size(); ++i)
	{
	    out << M.matchedSquares[i] << ", ";
	}
	
	return out;
    }

    vector<coord> matchedSquares;
};

class MultipleMatch : public vector<Match>{
public:
    bool matched(coord C){
	vector<Match>::iterator it;
	for(it = begin(); it != end(); ++it){
	    if(it -> find(C))
	       return true;
	}
	return false;
    }
    
};

/**
 * @class Board
 *
 * @brief Representa un board de juego en un momento dado.
 *
 * Contiene una matriz de 8x8 con el contenido de las casillas
 * además de algoritmos que permiten hacer comprobaciones sobre el board.
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


class Board{
public:
    Board();
    ~Board();

    /// Intercambia el valor de la casilla x1,y1 por el de x2,y2
    void swap(int x1, int y1, int x2, int y2);

    /// Vacía la casilla x,y
    void del(int x, int y);
    
    /// Genera un board nuevo completamente aleatorio
    void generate();

    /** Calcula las posiciones de las casillas tras borrar otras casillas. 

	Cuando se hace un grupo de tres o más casillas, esta función
	actualiza las casillas que estuvieran por encima para asignarles
	una nueva posición.
    */

    void calcFallMovements();
    // void calcularMovimientosCaida();


    /// Aplica los cambios calculados en la función calcularMovimientosCaida
    //void aplicarCaida();
    void applyFall();

    /// Rellena los espacios vacíos con nuevas casillas aleatorias
    //void rellenarEspacios();
    void fillSpaces();

    /**
     * @brief Comprueba si hay algún grupo de tres o más casillas en horizontal o vertical
     *
     * @return Conjunto de coordenadas con las casillas que forman parte de un grupo
     *
     */

    //vector<coord> comprobar();
    MultipleMatch check();

    /// Comprueba si existen movimientos con los que hacer algún grupo en el board actual
    ///vector<coord> existeSolucion();
    vector<coord> solutions();

    //void cancelarAnimaciones();
    void endAnimations();

    /// Matriz de casillas
    tr1::array< tr1::array<Square, 8>, 8> squares;
    friend ostream& operator <<(ostream& out, Board & B);
};
#endif /* _BOARD_H_ */

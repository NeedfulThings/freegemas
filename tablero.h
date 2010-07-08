#ifndef _TABLERO_H_
#define _TABLERO_H_

#include <Gosu/Gosu.hpp>

#include <vector>
#include <tr1/array>
using namespace std;

#include "log.h"

enum tCasilla { casVacia , casBlanca , casRoja, casLila, casNaranja, casVerde, casAmarilla, casAzul };

struct Casilla{
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

    int origY, destY;
    bool debeCaer;
};


struct coord{
    int x, y;
    coord(int x = 0, int y = 0) : x(x), y(y) { }
    bool operator ==(const coord & c){
	return (c.x == x && c.y == y);
    }
};

class Tablero{
public:
    Tablero();
    ~Tablero();

    tCasilla get(int x, int y);
    void swap(int x1, int y1, int x2, int y2);
    void del(int x, int y);
    
    void generar();

    /// Calcula las posiciones a las que tienen que caer las casillas al eliminar las premiadas
    void calcularMovimientosCaida();

    void aplicarCaida();

    void rellenarEspacios();

    vector<coord> comprobar();

    bool existeSolucion();

    tr1::array< tr1::array<Casilla, 8>, 8> casillas;
};
#endif /* _TABLERO_H_ */

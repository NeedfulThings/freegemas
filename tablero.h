#ifndef _TABLERO_H_
#define _TABLERO_H_

#include <Gosu/Gosu.hpp>

#include <vector>
#include <tr1/array>
using namespace std;

enum tCasilla { casVacia , casBlanca , casRoja, casLila, casNaranja, casVerde, casAmarilla, casAzul };

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
    void rellenarEspacios();

    vector<coord> comprobar();

    bool existeSolucion();
private:
    tr1::array< tr1::array<tCasilla, 8>, 8> casillas;
};
#endif /* _TABLERO_H_ */

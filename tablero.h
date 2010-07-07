#ifndef _TABLERO_H_
#define _TABLERO_H_

#include <Gosu/Gosu.hpp>

#include <tr1/array>
using namespace std;

enum tCasilla { casVacia , casBlanca , casRoja, casLila, casNaranja, casVerde, casAmarilla, casAzul };


class Tablero{
public:
    Tablero();
    ~Tablero();

    tCasilla get(int x, int y);
    tCasilla swap(int x1, int y1, int x2, int y2);

    void generar();
    bool comprobar();
    bool comprobarFila(int i);
    bool comprobarColumna(int i);
    bool existeSolucion();
private:
    tr1::array< tr1::array<tCasilla, 8>, 8> casillas;
};
#endif /* _TABLERO_H_ */

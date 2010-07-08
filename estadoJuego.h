/**
 * @file estadoJuego.h
 * 
 * @author José Tomás Tocino García
 * @date 2010
 *
 * Archivo para la clase de juego principal
 * 
 * Copyright (C) 2010 José Tomás Tocino García <theom3ga@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


#ifndef _ESTADOJUEGO_H_
#define _ESTADOJUEGO_H_

#include <Gosu/Gosu.hpp>

#include <utility>
#include <tr1/memory>
using namespace std;

#include "estado.h"
#include "tablero.h"

class Juego;

/**
 * @class EstadoJuego
 *
 * @brief Estado de juego principal, con tablero, gemas y demás.
 *
 * Este es el estado en el que el jugador puede interactuar con el tablero y jugar.
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


class EstadoJuego : public Estado{
public:
    EstadoJuego(Juego * p);
    void update();
    void draw();
    void buttonDown(Gosu::Button B);

    ~EstadoJuego();

private:

    /// Carga las imágenes de las gemas
    void cargarGemas();

    /// Indica si está sobre una gema
    bool sobreGema(int mX, int mY);

    /// Devuelve las coordenadas de casilla correspondientes a las coordenadas de ratón mX,mY.
    pair<int,int> dameGema(int mX, int mY);

    /// Regenera la imagen del marcador de puntos
    void repintarPuntos();

    Tablero tablero;

    enum tEstado{
	eEspera,
	eGemaMarcada,
	eGemasCambiando,
	eGemasDesapareciendo,
	eGemasNuevasCayendo
    };

    tEstado estado;

    /// Coordenadas de la primera casilla marcada
    int casillaMarcadaX, casillaMarcadaY;

    /// Coordenadas de la segunda casilla marcada
    int casillaMarcada2X, casillaMarcada2Y;

    /// Paso actual en la animación
    int pasoAnim;

    /// Total de pasos en la animación
    int totalAnim;

    /// Contenedor temporal de las casillas a borrar
    vector<coord> casillasGanadoras;

    tr1::shared_ptr<Gosu::Image> imgTablero;

    tr1::shared_ptr<Gosu::Image> imgBlanca;
    tr1::shared_ptr<Gosu::Image> imgRoja;
    tr1::shared_ptr<Gosu::Image> imgLila;
    tr1::shared_ptr<Gosu::Image> imgNaranja;
    tr1::shared_ptr<Gosu::Image> imgVerde;
    tr1::shared_ptr<Gosu::Image> imgAmarilla;
    tr1::shared_ptr<Gosu::Image> imgAzul;

    tr1::shared_ptr<Gosu::Image> imgSelector;

    tr1::shared_ptr<Gosu::Image> txtPuntos;
    int puntos;
};

#endif /* _ESTADOJUEGO_H_ */

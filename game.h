/**
 * @file juego.h
 * 
 * @author José Tomás Tocino García
 * @date 2010
 *
 * Fichero con la clase para la ventana principal
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

#ifndef _GAME_H_
#define _GAME_H_

#include <Gosu/Gosu.hpp>

#include <tr1/memory>
#include <string>
using namespace std;

#include "log.h"

class State;

/**
 * @class Juego
 *
 * @brief Ventana principal del juego.
 *
 * Se encarga de gestionar el autómata de estados
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */
class Game : public Gosu::Window{

public:
    Game ();

    void update();

    void draw();

    void buttonDown(Gosu::Button button);

    void changeState(string S);

    ~Game();

private:
    tr1::shared_ptr<State> currentState;
    string currentStateString;

    tr1::shared_ptr<Gosu::Image> mousePointer;

};

#endif /* _JUEGO_H_ */

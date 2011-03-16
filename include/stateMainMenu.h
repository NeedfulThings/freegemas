/**
 * @file stateMainMenu.h
 * 
 * @author José Tomás Tocino García
 * @date 2011
 *
 * Copyright (C) 2011 José Tomás Tocino García <theom3ga@gmail.com>
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


#ifndef _STATEMAINMENU_H_
#define _STATEMAINMENU_H_

#include <Gosu/Gosu.hpp>

#include "state.h"
#include "resManager.h"
#include "jewelGroupAnim.h"
#include "sdlfont.h"

#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <utility>
using namespace std;

class Game;

/**
 * @class StateMainMenu
 *
 * @brief 
 *
 * 
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com> 
 *
 */


class StateMainMenu : public State{
public:

    StateMainMenu(Game * p);

    void update();

    void draw();

    void buttonDown(Gosu::Button B);

    ~StateMainMenu();

private:



    enum transitionState{ TransitionIn,
			  Active,
			  TransitionOut };

    transitionState currentTransitionState;

    int animationCurrentStep;
    int animationLogoSteps;
    int animationTotalSteps;
			  
    boost::shared_ptr<Gosu::Image> imgBackground;
    boost::shared_ptr<Gosu::Image> imgLogo;
    boost::shared_ptr<Gosu::Image> imgHighl;

    boost::shared_ptr<Gosu::Font> font;
    JewelGroupAnim jewelAnim;

    // Menu configuration

    int menuSelectedOption;
    vector<pair<wstring, string> > menuOptions;
    void optionChosen();
    
    int menuYStart, menuYEnd, menuYGap;
    int menuXStart, menuXEnd;

};

#endif /* _STATEMAINMENU_H_ */

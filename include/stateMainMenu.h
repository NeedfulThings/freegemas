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

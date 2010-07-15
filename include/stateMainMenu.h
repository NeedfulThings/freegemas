#ifndef _STATEMAINMENU_H_
#define _STATEMAINMENU_H_

#include <Gosu/Gosu.hpp>

#include "state.h"

#include <boost/scoped_ptr.hpp>
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
			  
    boost::scoped_ptr<Gosu::Image> imgBackground;
    boost::scoped_ptr<Gosu::Image> imgLogo;

    boost::scoped_ptr<Gosu::Font> font;

};

#endif /* _STATEMAINMENU_H_ */

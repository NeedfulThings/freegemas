#ifndef _STATEMAINMENU_H_
#define _STATEMAINMENU_H_

#include <Gosu/Gosu.hpp>

#include "state.h"

#include <tr1/memory>
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
			  
    tr1::shared_ptr<Gosu::Image> imgBackground;
    tr1::shared_ptr<Gosu::Image> imgLogo;

    tr1::shared_ptr<Gosu::Font> font;

};

#endif /* _STATEMAINMENU_H_ */

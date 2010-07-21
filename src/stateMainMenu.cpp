#include "stateMainMenu.h"

#include "game.h"
#include "log.h"

#include "resManager.h"

template <typename T, typename R>

T clamp(T v, R bottom, R top){
    if(v > top) return top;
    if(v < bottom) return bottom;
    return v;
}

StateMainMenu::StateMainMenu(Game * p) : State(p){
    lDEBUG << Log::CON("StateMainMenu");
    
    currentTransitionState = TransitionIn;

    imgBackground = 
	ResourceManager::instance() -> getImage(Gosu::resourcePrefix() + 
						L"media/stateMainMenu/mainMenuBackground.png");

    imgLogo = 
	ResourceManager::instance() -> getImage(Gosu::resourcePrefix() + 
						L"media/stateMainMenu/mainMenuLogo.png");

    font =
	ResourceManager::instance() -> getFont(Gosu::resourcePrefix() + L"media/fuenteMenu.ttf", 30);

    animationTotalSteps = 30;
    animationLogoSteps = 30;
    animationCurrentStep = 0;
}

void StateMainMenu::update(){
    if(currentTransitionState == TransitionIn){
	animationCurrentStep ++;

	if(animationCurrentStep == animationTotalSteps){
	    currentTransitionState = Active;
	}

    } else if(currentTransitionState == Active){

    } else if(currentTransitionState == TransitionOut){

    }
}

void StateMainMenu::draw(){
    imgBackground -> draw(0,0,1);

    int logoAlfa = clamp( (int)(255 * (float)animationCurrentStep / animationLogoSteps),
				0, 255);

    imgLogo -> draw(86, 0, 2, 1, 1, 
		    Gosu::Color(logoAlfa, 255, 255, 255));

    int hor = 800 / 2 - font -> textWidth(L"Pulsa espacio para continuar") / 2;


    font -> draw(L"Pulsa espacio para continuar",
		 hor, 300, 2);
    font -> draw(L"Pulsa espacio para continuar",
		 hor, 302, 1.9, 1, 1, Gosu::Color(125, 0,0,0));
}

void StateMainMenu::buttonDown(Gosu::Button B){
    if(B == Gosu::kbEscape){
	parent -> close();

    } else if(B == Gosu::kbSpace){
	parent -> changeState("stateGame");
    }
}

StateMainMenu::~StateMainMenu(){
    lDEBUG << Log::DES("StateMainMenu");
}

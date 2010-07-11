 #include "game.h"

 #include "log.h"
 #include "state.h"

 #include "stateGame.h"


Game::Game () : Gosu::Window(800, 600, false) {
    lDEBUG << Log::CON("Game");
    setCaption(L"FreeGemas v0.1");
    
    mousePointer.reset(new Gosu::Image(graphics(), L"media/handCursor.png"));
    
    changeState("estadoGame");
}

void Game::update(){
    currentState -> update();
}

void Game::draw(){
    mousePointer -> draw(input().mouseX(), input().mouseY(), 90);
    currentState -> draw();
}

void Game::buttonDown(Gosu::Button button){
    currentState -> buttonDown(button);
}

void Game::changeState(string S){
    if(S == currentStateString)
	return;

    if(S == "estadoGame"){
	currentState . reset(new StateGame(this));
	currentStateString = "estadoGame";
    }
}

Game::~Game(){
    lDEBUG << Log::DES("Game");
}

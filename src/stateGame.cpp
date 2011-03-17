#include "stateGame.h"
#include "log.h"
#include "game.h"
#include "resManager.h"
#include "inter.h"

#include <cmath>
#include <algorithm>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#define foreach BOOST_FOREACH

using boost::format;

StateGame::StateGame(Game * p) : State(p){
    lDEBUG << Log::CON("StateGame");
    state = eLoading;

    imgLoadingBanner = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/loadingBanner.png");

}

void StateGame::init(){
    state = eInicialGemas;
    
    fontTime = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fuentelcd.ttf", 62);
    fontScore = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fuentelcd.ttf", 33);

    imgBoard = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/board.png");
    imgSelector = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/selector.png");    

    imgTimeBackground = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/timeBackground.png");
    imgScoreBackground = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/scoreBackground.png");
    
    Gosu::Bitmap bmpTemp;
    bmpTemp = Gosu::createText(Gosu::utf8ToWstring(_("puntos")),
                               Gosu::resourcePrefix() + L"media/fNormal.ttf", 
                               37, 0, 190, Gosu::taCenter);
    imgScoreHeader.reset(new Gosu::Image(parent -> graphics(), bmpTemp));

    bmpTemp = Gosu::createText(Gosu::utf8ToWstring(_("tiempo restante")),
                               Gosu::resourcePrefix() + L"media/fNormal.ttf", 
                               37, 0, 190, Gosu::taCenter);
    imgTimeHeader.reset(new Gosu::Image(parent -> graphics(), bmpTemp));


    hintButton.reset(new BaseButton(parent -> graphics(),
                                    Gosu::utf8ToWstring(_("Mostrar pista")), L"iconHint.png"));

    resetButton.reset(new BaseButton(parent -> graphics(),
                                     Gosu::utf8ToWstring(_("Reiniciar juego")), L"iconRestart.png"));

    exitButton.reset(new BaseButton(parent -> graphics(),
                                    Gosu::utf8ToWstring(_("Salir")), L"iconExit.png"));

    musicButton.reset(new BaseButton(parent -> graphics(),
                                     Gosu::utf8ToWstring(_("Encender música")), L"iconMusic.png"));



    // Sound loading
    sfxMatch1.reset(new Gosu::Sample(Gosu::resourcePrefix() + L"media/match1.ogg"));
    sfxMatch2.reset(new Gosu::Sample(Gosu::resourcePrefix() + L"media/match2.ogg"));
    sfxMatch3.reset(new Gosu::Sample(Gosu::resourcePrefix() + L"media/match3.ogg"));
    sfxSelect.reset(new Gosu::Sample(Gosu::resourcePrefix() + L"media/select.ogg"));
    sfxFall.reset(new Gosu::Sample(Gosu::resourcePrefix() + L"media/fall.ogg"));
    sfxSong.reset(new Gosu::Song(Gosu::resourcePrefix() + L"media/music1.ogg"));
    
    selectedSquareFirst.x = -1;
    selectedSquareFirst.y = -1;
    //17 195

    loadGems();

    pasoAnim = 0;
    totalAnim = 17;
    totalAnimInit = 50;

    redrawScoreboard();

    mostrandoPista = -1;
    totalAnimPista = 50;

    acumulator = 1;
    
    resetGame();

    //sfxSong -> play(true);
    sfxSong -> changeVolume(0.5);    
}

void StateGame::resetGame(){
    puntos = 0;
    redrawScoreboard();
    timeStart = Gosu::milliseconds() + 2 * 60 * 1000;
}

void StateGame::redrawScoreboard(){
    Gosu::Bitmap temporal = Gosu::createText(boost::lexical_cast<wstring>(puntos),
                                             Gosu::resourcePrefix() + L"media/fuentelcd.ttf", 33, 0, 190, Gosu::taRight);

    txtPuntos.reset(new Gosu::Image(parent -> graphics(), temporal));
}

void StateGame::playMatchSound(){
    if(acumulator == 1){
        sfxMatch1 -> play(0.35);
    }else if(acumulator == 2){
        sfxMatch2 -> play(0.35);
    }else{
        sfxMatch3 -> play(0.35);
    }
}

void StateGame::createFloatingScores(){
    foreach(Match & m, groupedSquares){
        scoreSet.push_back(FloatingScore(parent -> graphics(),
                                         m.size() * 5 * acumulator,
                                         m.midSquare().x,
                                         m.midSquare().y));
        puntos += m.size() * 5 * acumulator;       
    }

    redrawScoreboard();
}
void StateGame::update(){

    if(state == eLoading)
        return;

    if(state == eFirstFlip){
        init();
        return;
    }

    double timeDiff = (timeStart - Gosu::milliseconds()) / 1000;
	
    if(timeDiff >= 0){
        int minutes = int(timeDiff / 60);
        int seconds = int(timeDiff - minutes * 60);
        txtTime = boost::lexical_cast<string>(minutes) + 
            (seconds < 10 ? ":0" : ":") + 
            boost::lexical_cast<string>(seconds);
    }
    else if(timeDiff < 0 && state != eTimeFinished && state != eShowingScoreTable){
        state = eTimeFinished;
        gemsOutScreen();
    }

    scoreSet.erase(remove_if(scoreSet.begin(), scoreSet.end(), 
                             boost::bind<bool>(&FloatingScore::ended, _1)), scoreSet.end());

    if(state == eInicialGemas){
        if(++pasoAnim == totalAnimInit){

            state = eEspera;
            board.endAnimations();

            pasoAnim = 0;
        }
    }

    else if(state == eEspera){
        acumulator = 0;
    }

    else if(state == eGemasCambiando){	
        if(++pasoAnim == totalAnim){

            state = eGemasDesapareciendo;
            board.swap(selectedSquareFirst.x, selectedSquareFirst.y,
                       selectedSquareSecond.x, selectedSquareSecond.y);

            ++acumulator;
            playMatchSound();
            createFloatingScores();
            pasoAnim = 0;
        }
    }

    else if(state == eGemasDesapareciendo){
        if(pasoAnim++ == totalAnim){

            state = eGemasNuevasCayendo;

            //TO-DO
            //puntos += 10 * groupedSquares.size();
            redrawScoreboard();

            for(size_t i = 0; i < groupedSquares.size(); ++i){
                for(size_t j = 0; j < groupedSquares[i].size(); ++j){
                    board.del(groupedSquares[i][j].x,
                              groupedSquares[i][j].y);
                }
            }

            // Calculando los movimientos de caída...

            board.calcFallMovements();

            //Aplicando las modificaciones según caídas a la matriz de squares...

            board.applyFall();

            // Rellenando los espacios que han quedado por arriba...

            board.fillSpaces();

            pasoAnim = 0;
        }
    }

    else if(state == eGemasNuevasCayendo){
        if(pasoAnim++ == totalAnim){
            sfxFall -> play(0.3);
            state = eEspera;
            pasoAnim = 0;

            for(int x = 0; x < 8; ++x){
                for(int y = 0; y < 8; ++y){
                    board.squares[x][y].origY = y;
                    board.squares[x][y].destY = 0;
                    board.squares[x][y].mustFall = false;
                }
            }

            // Fin de turno, pero comprobamos posibles movimientos ganadores indirectos...

            groupedSquares = board.check();

            if(! groupedSquares.empty()){
                // Si encontramos más filas o columnas
                ++acumulator;
                createFloatingScores();
                playMatchSound();
                state = eGemasDesapareciendo;
            }

            else if(board.solutions().empty()){

                // Si el tablero no tiene más juegos posibles

                lDEBUG << Log::cRojo << "ZOMG NO EXISTEN MÁS MOVIMIENTOS";

                state = eDesapareceBoard;
                gemsOutScreen();

            }
        }
    }

    else if(state == eDesapareceBoard){
        if(++pasoAnim == totalAnimInit){

            state = eInicialGemas;
            board.generate();

            pasoAnim = 0;
        }
    }

    else if(state == eTimeFinished){
        if(++pasoAnim == totalAnimInit){
            scoreTable.reset(new ScoreTable(parent, puntos));
            state = eShowingScoreTable;
			 
            pasoAnim = 0;
        }
    }

    if(mostrandoPista != -1) mostrandoPista --;

}

/*
  tiempo pasado, beginning, change, duration
*/
float StateGame::eqMovIn(float t, float b, float c, float d) {
    t/=d;
    return c*t*t + b;
}

float StateGame::eqMovOut(float t, float b, float c, float d) {
    t/=d;
    return -c *(t)*(t-2) + b;
}


void StateGame::draw(){
    if(state == eLoading || state == eFirstFlip){
        state = eFirstFlip;
        imgLoadingBanner -> draw(156, 200, 2);
        return;
    }


    imgBoard -> draw(0,0,1);

    int vertButStart = 360;

    hintButton -> draw(17, vertButStart, 2);
    resetButton -> draw(17, vertButStart + 47, 2);
    musicButton -> draw(17, vertButStart + 47 * 2, 2);

    exitButton -> draw(17, 538, 2);

    imgScoreBackground -> draw(17, 124, 2);
    imgScoreHeader -> draw(17, 84, 3, 1, 1, Gosu::Color(0xffa0a9ff));
    imgScoreHeader -> draw(18, 85, 2, 1, 1, Gosu::Color(0x66000000));

    imgTimeBackground -> draw(17, 230, 2);
    imgTimeHeader -> draw(17, 190, 3, 1, 1, Gosu::Color(0xffa0a9ff));
    imgTimeHeader -> draw(18, 191, 2, 1, 1, Gosu::Color(0x66000000));

    fontTime -> draw(Gosu::utf8ToWstring(txtTime), 
                     190 - fontTime ->textWidth(Gosu::utf8ToWstring(txtTime)),
                     232, 2, 1, 1,
                     Gosu::Color(0xff4ec1be));

    fontScore -> draw(boost::lexical_cast<wstring>(puntos), 
                      197 - fontScore ->textWidth(boost::lexical_cast<wstring>(puntos)),
                      127, 2, 1, 1,
                      Gosu::Color(0xff4ec1be));

    //*/

    int posX = 241, 
        posY = 41;

    std::for_each(scoreSet.begin(), scoreSet.end(), boost::bind(&FloatingScore::draw, _1));

    //txtPuntos -> draw(8, 127, 5, 1, 1, Gosu::Color(0xff4ec1be));

    boost::shared_ptr<Gosu::Image> img;

    if(state != eShowingScoreTable){
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                switch(board.squares[i][j]){
                case sqWhite:
                    img = imgWhite;
                    break;

                case sqRed:
                    img = imgRed;
                    break;

                case sqPurple:
                    img = imgPurple;
                    break;

                case sqOrange:
                    img = imgOrange;
                    break;

                case sqGreen:
                    img = imgGreen;
                    break;

                case sqYellow:
                    img = imgYellow;
                    break;

                case sqBlue:
                    img = imgBlue;
                    break;

                case sqEmpty:
                    img.reset();
                    break;
                } // fin switch

                if(img != NULL){
                    if(state == eInicialGemas){
                        img -> draw(posX + i * 65,
                                    eqMovOut(float(pasoAnim),
                                             float(posY + board.squares[i][j].origY * 65),
                                             float(board.squares[i][j].destY * 65),
                                             float(totalAnimInit)),
                                    3);
                    }
                    else if(state == eDesapareceBoard || state == eTimeFinished){
                        img -> draw(posX + i * 65,
                                    eqMovIn(float(pasoAnim),
                                            float(posY + board.squares[i][j].origY * 65),
                                            float(board.squares[i][j].destY * 65),
                                            float(totalAnimInit)),
                                    3);
                    }

                    else if(state == eEspera || state == eGemaMarcada ||
                            state == eGemasNuevasCayendo){

                        if(board.squares[i][j].mustFall){
                            img -> draw(posX + i * 65,
                                        eqMovOut(float(pasoAnim),
                                                 float(posY + board.squares[i][j].origY * 65),
                                                 float(board.squares[i][j].destY * 65),
                                                 float(totalAnim)),
                                        3);
                        }else{
                            img -> draw(posX + i * 65,
                                        posY + j * 65,
                                        3);
                        }
                    }


                    else if(state == eGemasCambiando){
                        if(i == selectedSquareFirst.x && 
                           j == selectedSquareFirst.y){

                            img -> draw(eqMovOut(float(pasoAnim),
                                                 float(posX + i * 65),
                                                 float((selectedSquareSecond.x - selectedSquareFirst.x) * 65),
                                                 float(totalAnim)),

                                        eqMovOut(float(pasoAnim),
                                                 float(posY + j * 65),
                                                 float((selectedSquareSecond.y - selectedSquareFirst.y) * 65),
                                                 float(totalAnim)),

                                        3);

                        }

                        else if(i == selectedSquareSecond.x && 
                                j == selectedSquareSecond.y){

                            img -> draw(eqMovOut(float(pasoAnim),
                                                 float(posX + i * 65),
                                                 float((selectedSquareFirst.x - selectedSquareSecond.x) * 65),
                                                 float(totalAnim)),

                                        eqMovOut(float(pasoAnim),
                                                 float(posY + j * 65),
                                                 float((selectedSquareFirst.y - selectedSquareSecond.y) * 65),
                                                 float(totalAnim)),

                                        3);

                        }

                        else{
                            img -> draw(posX + i * 65,
                                        posY + j * 65,
                                        3);
                        }

                    }

                    else if(state == eGemasDesapareciendo){
                        // Desaparición de las gemas ganadoras

                        if(groupedSquares.matched(coord(i,j))){

                            img -> draw(posX + i * 65,
                                        posY + j * 65,
                                        3, 1, 1,
                                        Gosu::Color(int(255 * (1 -(float)pasoAnim/totalAnim)), 
                                                    255, 255, 255));
                        }
                        else{
                            img -> draw(posX + i * 65,
                                        posY + j * 65,
                                        3);
                        }


                    }
                } // Fin if (img != NULL)

                img.reset();
            }
        }

        int mX = (int) parent -> input().mouseX();
        int mY = (int) parent -> input().mouseY();

        if(overGem(mX, mY) ){
            imgSelector -> draw( 241 + getCoord(mX, mY).x * 65,
                                 41 + getCoord(mX, mY).y * 65,
                                 4);
        }

        if(state == eGemaMarcada){
            imgSelector -> draw(241 + selectedSquareFirst.x * 65,
                                41 + selectedSquareFirst.y * 65,
                                4, 1, 1,
                                Gosu::Color(0xffff0000));
        }

        if(mostrandoPista != -1){
            float p1 = (float)mostrandoPista / totalAnimPista;

            float pX1 = 241 + coordPista.x * 65 - imgSelector -> width() * (2 - p1) / 2 + 65 / 2;
            float pY1 = 41 + coordPista.y * 65 - imgSelector -> height() * (2 - p1) / 2 + 65 / 2;

            imgSelector -> draw(pX1, pY1,
                                3, 2 - p1, 2 - p1,
                                Gosu::Color(int(p1 * 255), 0, 255, 0));

        }
    }else{
        scoreTable -> draw(241 + (65 * 8) / 2 - 150  , 105, 3);
    }
}

bool StateGame::overGem(int mX, int mY){
    return (mX > 241 && mX < 241 + 65 * 8 &&
            mY > 41 && mY < 41 + 65 * 8);
}

coord StateGame::getCoord(int mX, int mY){
    return coord((mX - 241) / 65 , 
                 (mY - 41) / 65 );
}

bool StateGame::checkClickedSquare(int mX, int mY){
    selectedSquareSecond = getCoord(mX, mY);

    bool returnValue = false;

    // Si es una gema CONTIGUA
    if(abs(selectedSquareFirst.x - selectedSquareSecond.x) 
       + abs(selectedSquareFirst.y - selectedSquareSecond.y) == 1){ 

        Board temporal = board;
        temporal.swap(selectedSquareFirst.x, selectedSquareFirst.y,
                      selectedSquareSecond.x, selectedSquareSecond.y);

        groupedSquares = temporal.check();

        // SI ES UN MOVIMIENTO GANADOR
        if(! groupedSquares.empty()){
            state = eGemasCambiando;
            returnValue = true;
        }else{
            returnValue = false;
        }
    }

    return returnValue;
}

void StateGame::buttonUp (Gosu::Button B){
    if(B == Gosu::msLeft){
        clicking = false;

        if(state == eGemaMarcada){
            int mX = (int) parent -> input().mouseX();
            int mY = (int) parent -> input().mouseY();

            coord res = getCoord(mX, mY);

            if(!(res == selectedSquareFirst)){
                checkClickedSquare(mX, mY);
            }
        }
    }
}

void StateGame::buttonDown (Gosu::Button B){
    if(B == Gosu::kbEscape){
        parent -> changeState("stateMainMenu");
    }

    else if(B == Gosu::msLeft){ // Se pulsó el ratón
        clicking = true;

        int mX = (int) parent -> input().mouseX();
        int mY = (int) parent -> input().mouseY();

        if(exitButton -> clicked(mX, mY)){
            parent -> changeState("stateMainMenu");
        }

        else if(hintButton -> clicked(mX, mY)){
            showHint();
        }

        else if(musicButton -> clicked(mX, mY)){
            if(sfxSong -> playing()){
                musicButton -> changeText(Gosu::utf8ToWstring(_("Encender música")));
                sfxSong -> stop();
            }else{
                musicButton -> changeText(Gosu::utf8ToWstring(_("Apagar música")));
                sfxSong -> play(true);
            }	    
        }
        else if (resetButton -> clicked(mX, mY)){
            state = eDesapareceBoard;
            gemsOutScreen();
            resetGame();
	    
        }
        else if(overGem(mX, mY)){ // Si se pulsó sobre una gema
            sfxSelect -> play(0.3);

            if(state == eEspera){ // Si no hay ninguna gema marcada
                state = eGemaMarcada;
                selectedSquareFirst.x = getCoord(mX, mY).x;
                selectedSquareFirst.y = getCoord(mX, mY).y;
            }

            else if(state == eGemaMarcada){ // Si ya había una gema marcada
                if(!checkClickedSquare(mX, mY)){
                    selectedSquareFirst.x = -1;
                    selectedSquareFirst.y = -1;
                    state = eEspera;		    
                }
            }
        }
    }

    else if(B == Gosu::kbH){
        showHint();
    }

    if(state == eShowingScoreTable){
        scoreTable -> buttonDown(B);
    }
}

void StateGame::showHint(){
    vector<coord> posibilidades = board.solutions();
    coordPista = posibilidades[0];
    lDEBUG << "showHint: " << coordPista;
    mostrandoPista = totalAnimPista;   
}

void StateGame::gemsOutScreen(){
    for(int x = 0; x < 8; ++x){
        for(int y = 0; y < 8; ++y){
            board.squares[x][y].mustFall = true;
            board.squares[x][y].origY = y;
            board.squares[x][y].destY = int(9 + Gosu::random(1,7));
        }
    }
}
StateGame::~StateGame(){
    lDEBUG << Log::DES("StateGame");
}

void StateGame::loadGems(){

    imgWhite = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemWhite.png");
    imgRed = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemRed.png");
    imgPurple = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemPurple.png");
    imgOrange = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemOrange.png");
    imgGreen = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemGreen.png");
    imgYellow = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemYellow.png");
    imgBlue = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemBlue.png");

}

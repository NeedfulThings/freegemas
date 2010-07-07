#include "estadoJuego.h"
#include "log.h"
#include "juego.h"

EstadoJuego::EstadoJuego(Juego * p) : Estado(p){
    lDEBUG << Log::CON("EstadoJuego");

    imgTablero.reset(new Gosu::Image(padre -> graphics(),
				  L"media/tablero.png"));
    
    lDEBUG << "¿Existe solución? " << tablero.existeSolucion();
    cargarGemas();
}

void EstadoJuego::update(){

}

void EstadoJuego::draw(){
    imgTablero -> draw(0,0,1);
    tCasilla c;

    int posX = 241, 
	posY = 41;

    tr1::shared_ptr<Gosu::Image> img;

    for(int i = 0; i < 8; ++i){
	for(int j = 0; j < 8; ++j){
	    switch(tablero.get(i,j)){
	    case casBlanca:
		img = imgBlanca;
		break;

	    case casRoja:
		img = imgRoja;
		break;

	    case casLila:
		img = imgLila;
		break;

	    case casNaranja:
		img = imgNaranja;
		break;

	    case casVerde:
		img = imgVerde;
		break;

	    case casAmarilla:
		img = imgAmarilla;
		break;

	    case casAzul:
		img = imgAzul;
		break;
	    } // fin switch

	    img -> draw(posX + i * 65,
			posY + j * 65,
			3);
	}
    }


}

void EstadoJuego::buttonDown(Gosu::Button B){
    if(B == Gosu::kbEscape){
	padre -> close();
    }
}

EstadoJuego::~EstadoJuego(){
    lDEBUG << Log::DES("EstadoJuego");
}

void EstadoJuego::cargarGemas(){
    Gosu::Graphics & g = padre -> graphics();

    imgBlanca.reset(new Gosu::Image(g, L"media/gemBlanca.png"));
    imgRoja.reset(new Gosu::Image(g, L"media/gemRoja.png"));
    imgLila.reset(new Gosu::Image(g, L"media/gemLila.png"));
    imgNaranja.reset(new Gosu::Image(g, L"media/gemNaranja.png"));
    imgVerde.reset(new Gosu::Image(g, L"media/gemVerde.png"));
    imgAmarilla.reset(new Gosu::Image(g, L"media/gemAmarilla.png"));
    imgAzul.reset(new Gosu::Image(g, L"media/gemAzul.png"));

}

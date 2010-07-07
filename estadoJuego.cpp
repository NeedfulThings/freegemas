#include "estadoJuego.h"
#include "log.h"
#include "juego.h"

#include <cmath>


EstadoJuego::EstadoJuego(Juego * p) : Estado(p){
    lDEBUG << Log::CON("EstadoJuego");

    imgTablero.reset(new Gosu::Image(padre -> graphics(),
				  L"media/tablero.png"));

    imgSelector.reset(new Gosu::Image(padre -> graphics(), 
				      L"media/selector.png"));    

    estado = eEspera;
    casillaMarcadaX = -1;
    casillaMarcadaY = -1;


    cargarGemas();
}

void EstadoJuego::update(){

}

void EstadoJuego::draw(){
    imgTablero -> draw(0,0,1);

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

	    case casVacia:
		img.reset();
		break;
	    } // fin switch

	    if(img != NULL){
		img -> draw(posX + i * 65,
			    posY + j * 65,
			    3);
	    }

	    img.reset();
	}
    }

    int mX = padre -> input().mouseX();
    int mY = padre -> input().mouseY();

    if(sobreGema(mX, mY) ){
	imgSelector -> draw( 241 + dameGema(mX, mY).first * 65,
			     41 + dameGema(mX, mY).second * 65,
			     4);
    }

    if(estado == eGemaMarcada){
	imgSelector -> draw(241 + casillaMarcadaX * 65,
			    41 + casillaMarcadaY * 65,
			    4);
    }
}

bool EstadoJuego::sobreGema(int mX, int mY){
    return (mX > 241 && mX < 241 + 65 * 8 &&
	    mY > 41 && mY < 41 + 65 * 8);
}

pair<int,int> EstadoJuego::dameGema(int mX, int mY){
    return make_pair((mX - 241) / 65 , 
		     (mY - 41) / 65 );
}

void EstadoJuego::buttonDown(Gosu::Button B){
    if(B == Gosu::kbEscape){
	padre -> close();
    }

    else if(B == Gosu::msLeft){ // Se pulsó el ratón

	int mX = padre -> input().mouseX();
	int mY = padre -> input().mouseY();

	if(sobreGema(mX, mY)){ // Si se pulsó sobre una gema
	
	    if(estado == eEspera){ // Si no hay ninguna gema marcada
		estado = eGemaMarcada;
		casillaMarcadaX = dameGema(mX, mY).first;
		casillaMarcadaY = dameGema(mX, mY).second;
	    }

	    else if(estado == eGemaMarcada){ // Si ya había una gema marcada
		pair<int,int> res = dameGema(mX, mY);

		int selecX = res.first;
		int selecY = res.second;

		if(abs(casillaMarcadaX - selecX) + abs(casillaMarcadaY - selecY) == 1){ 

		    lDEBUG << "Casilla contigua";

		    Tablero temporal = tablero;
		    temporal.swap(casillaMarcadaX, casillaMarcadaY,
				  selecX, selecY);

		    vector<coord> casillasGanadoras = temporal.comprobar();
		    if(!casillasGanadoras.empty()){
			lDEBUG << "Movimiento ganador!";

			tablero.swap(casillaMarcadaX, casillaMarcadaY,
				  selecX, selecY);

			for(size_t i = 0; i < casillasGanadoras.size(); ++i){
			    tablero.del(casillasGanadoras[i].x,
					casillasGanadoras[i].y);
			}
		    }
		}

		casillaMarcadaX = -1;
		casillaMarcadaY = -1;
		estado = eEspera;		    
	    }
	}

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

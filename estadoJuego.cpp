#include "estadoJuego.h"
#include "log.h"
#include "juego.h"

#include <cmath>
#include <algorithm>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

using boost::format;

EstadoJuego::EstadoJuego(Juego * p) : Estado(p){
    lDEBUG << Log::CON("EstadoJuego");

    imgTablero.reset(new Gosu::Image(padre -> graphics(),
				     L"media/tablero.png"));

    imgSelector.reset(new Gosu::Image(padre -> graphics(), 
				      L"media/selector.png"));    

    estado = eInicialGemas;

    casillaMarcadaX = -1;
    casillaMarcadaY = -1;


    cargarGemas();

    pasoAnim = 0;
    totalAnim = 17;
    totalAnimInit = 50;

    puntos = 0;

    repintarPuntos();

    mostrandoPista = -1;
    totalAnimPista = 50;
}

void EstadoJuego::repintarPuntos(){
    Gosu::Bitmap temporal = Gosu::createText(boost::lexical_cast<wstring>(puntos),
					     L"media/fuentelcd.ttf", 33, 0, 190, Gosu::taRight);

    txtPuntos.reset(new Gosu::Image(padre -> graphics(),
				    temporal));
}

void EstadoJuego::update(){
    if(estado == eInicialGemas){
	if(++pasoAnim == totalAnimInit){
	    lDEBUG << "Fichas iniciales colocadas";

	    estado = eEspera;
	    tablero.cancelarAnimaciones();

	    pasoAnim = 0;
	}
    }

    else if(estado == eGemasCambiando){	
	if(++pasoAnim == totalAnim){
	    lDEBUG << "Fin eGemasCambiando";

	    estado = eGemasDesapareciendo;
	    tablero.swap(casillaMarcadaX, casillaMarcadaY,
			 casillaMarcada2X, casillaMarcada2Y);

	    pasoAnim = 0;
	}
    }

    else if(estado == eGemasDesapareciendo){
	if(pasoAnim++ == totalAnim){
	    lDEBUG << "Fin eGemasDesapareciendo";

	    estado = eGemasNuevasCayendo;
	    
	    puntos += 10 * casillasGanadoras.size();
	    repintarPuntos();

	    for(size_t i = 0; i < casillasGanadoras.size(); ++i){
		tablero.del(casillasGanadoras[i].x,
			    casillasGanadoras[i].y);
	    }

	    lDEBUG << Log::cAmar << "Calculando los movimientos de caída...";

	    tablero.calcularMovimientosCaida();

	    lDEBUG << Log::cAmar << "Aplicando las modificaciones según caídas a la matriz de casillas...";
	    tablero.aplicarCaida();

	    lDEBUG << Log::cAmar << "Rellenando los espacios que han quedado por arriba";
	    tablero.rellenarEspacios();

	    pasoAnim = 0;
	}
    }

    else if(estado == eGemasNuevasCayendo){
	if(pasoAnim++ == totalAnim){
	    lDEBUG << "Fin eGemasNuevasCayendo";

	    estado = eEspera;
	    pasoAnim = 0;

	    for(int x = 0; x < 8; ++x){
		for(int y = 0; y < 8; ++y){
		    tablero.casillas[x][y].origY = y;
		    tablero.casillas[x][y].destY = 0;
		    tablero.casillas[x][y].debeCaer = false;
		}
	    }

	    lDEBUG << Log::cAmar << "Fin de turno, pero comprobamos posibles movimientos ganadores indirectos";

	    casillasGanadoras = tablero.comprobar();
	    if(! casillasGanadoras.empty()){
		lDEBUG << "Movimiento ganador!";
		
		estado = eGemasDesapareciendo;
	    }else if(tablero.existeSolucion().empty()){
		lDEBUG << Log::cRojo << "ZOMG NO EXISTEN MÁS MOVIMIENTOS";
		
		estado = eDesapareceTablero;

		for(int x = 0; x < 8; ++x){
		    for(int y = 0; y < 8; ++y){
			tablero.casillas[x][y].debeCaer = true;
			tablero.casillas[x][y].origY = y;
			tablero.casillas[x][y].destY = 7 + Gosu::random(1,7);
		    }
		}
	    }
	}
    }

    else if(estado == eDesapareceTablero){
	if(++pasoAnim == totalAnimInit){
	    lDEBUG << "Tablero desaparecido";

	    estado = eInicialGemas;
	    tablero.generar();

	    pasoAnim = 0;
	}
    }

}

/*
  tiempo pasado, beginning, change, duration
 */
float EstadoJuego::eqMov(float t, float b, float c, float d) {
    t/=d;
    return -c *(t)*(t-2) + b;
};


void EstadoJuego::draw(){
    imgTablero -> draw(0,0,1);

    int posX = 241, 
	posY = 41;

    tr1::shared_ptr<Gosu::Image> img;

    for(int i = 0; i < 8; ++i){
	for(int j = 0; j < 8; ++j){
	    switch(tablero.casillas[i][j]){
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
		if(estado == eInicialGemas || estado == eDesapareceTablero){
		    img -> draw(posX + i * 65,
				eqMov(pasoAnim,
				      posY + tablero.casillas[i][j].origY * 65,
				      tablero.casillas[i][j].destY * 65,
				      totalAnimInit),
				3);
		}
		else if(estado == eEspera || estado == eGemaMarcada || estado == eGemasNuevasCayendo){

		    if(tablero.casillas[i][j].debeCaer){
			img -> draw(posX + i * 65,
				    eqMov(pasoAnim,
					  posY + tablero.casillas[i][j].origY * 65,
					  tablero.casillas[i][j].destY * 65,
					  totalAnim),
				    3);
		    }else{
			img -> draw(posX + i * 65,
				    posY + j * 65,
				    3);
		    }
		}
	    

		else if(estado == eGemasCambiando){
		    if(i == casillaMarcadaX && 
		       j == casillaMarcadaY){

			img -> draw(eqMov(pasoAnim,
					  posX + i * 65,
					  (casillaMarcada2X - casillaMarcadaX) * 65,
					  totalAnim),

				    eqMov(pasoAnim,
					  posY + j * 65,
					  (casillaMarcada2Y - casillaMarcadaY) * 65,
					  totalAnim),

				    3);

		    }
		
		    else if(i == casillaMarcada2X && 
			    j == casillaMarcada2Y){
		
			img -> draw(eqMov(pasoAnim,
					  posX + i * 65,
					  (casillaMarcadaX - casillaMarcada2X) * 65,
					  totalAnim),

				    eqMov(pasoAnim,
					  posY + j * 65,
					  (casillaMarcadaY - casillaMarcada2Y) * 65,
					  totalAnim),

				    3);

		    }

		    else{
			img -> draw(posX + i * 65,
				    posY + j * 65,
				    3);
		    }
		
		}

		else if(estado == eGemasDesapareciendo){
		    // Desaparición de las gemas ganadoras
		    
		    if(find(casillasGanadoras.begin(), 
			    casillasGanadoras.end(), 
			    coord(i, j)) != casillasGanadoras.end() ){
			
			img -> draw(posX + i * 65,
				    posY + j * 65,
				    3, 1, 1,
				    Gosu::Color(255 * (1 -(float)pasoAnim/totalAnim), 255, 255, 255));
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
			    4, 1, 1,
			    Gosu::Color(0xffff0000));
    }

    if(mostrandoPista != -1){
	imgSelector -> draw(241 + coordPista.x * 65,
			    41 + coordPista.y * 65,
			    3, 1, 1,
			    Gosu::Color((float)mostrandoPista / totalAnimPista * 255, 0, 255, 0));
	mostrandoPista --;
    }

    txtPuntos -> draw(8, 127, 5, 1, 1, Gosu::Color(0xff4ec1be));
}

bool EstadoJuego::sobreGema(int mX, int mY){
    return (mX > 241 && mX < 241 + 65 * 8 &&
	    mY > 41 && mY < 41 + 65 * 8);
}

pair<int,int> EstadoJuego::dameGema(int mX, int mY){
    return make_pair((mX - 241) / 65 , 
		     (mY - 41) / 65 );
}

void EstadoJuego::buttonDown (Gosu::Button B){
    if(B == Gosu::kbEscape){
	padre -> close();
    }

    else if(B == Gosu::msLeft){ // Se pulsó el ratón

	int mX = padre -> input().mouseX();
	int mY = padre -> input().mouseY();
	lDEBUG << "Click @ " << mX << "," << mY;

	if(sobreGema(mX, mY)){ // Si se pulsó sobre una gema
	
	    if(estado == eEspera){ // Si no hay ninguna gema marcada
		estado = eGemaMarcada;
		casillaMarcadaX = dameGema(mX, mY).first;
		casillaMarcadaY = dameGema(mX, mY).second;
	    }

	    else if(estado == eGemaMarcada){ // Si ya había una gema marcada
		pair<int,int> res = dameGema(mX, mY);

		casillaMarcada2X = res.first;
		casillaMarcada2Y = res.second;

		// Si es una gema CONTIGUA
		if(abs(casillaMarcadaX - casillaMarcada2X) 
		   + abs(casillaMarcadaY - casillaMarcada2Y) == 1){ 

		    lDEBUG << "Casilla contigua";

		    Tablero temporal = tablero;
		    temporal.swap(casillaMarcadaX, casillaMarcadaY,
				  casillaMarcada2X, casillaMarcada2Y);

		    casillasGanadoras = temporal.comprobar();
		    
		    // SI ES UN MOVIMIENTO GANADOR
		    if(! casillasGanadoras.empty()){
			lDEBUG << "Movimiento ganador!";

			estado = eGemasCambiando;
		    }
		    
		    // SI ES CONTIGUA pero no es un movimiento ganador
		    else{
			casillaMarcadaX = -1;
			casillaMarcadaY = -1;
			estado = eEspera;		    
		    }
		}
		
		// Si NO ES contigua
		else{
		    casillaMarcadaX = -1;
		    casillaMarcadaY = -1;
		    estado = eEspera;		    
		}
	    }
	}

    }

    else if(B == Gosu::kbH){
	lDEBUG << "Hint...";
	vector<coord> posibilidades = tablero.existeSolucion();
	if(posibilidades.empty()){

	}else{
	    coordPista = posibilidades[0];
	    mostrandoPista = totalAnimPista;
	}	
    }

    else if(B == Gosu::kbP){
	estado = eDesapareceTablero;

	for(int x = 0; x < 8; ++x){
	    for(int y = 0; y < 8; ++y){
		tablero.casillas[x][y].debeCaer = true;
		tablero.casillas[x][y].origY = y;
		tablero.casillas[x][y].destY = 8 + Gosu::random(1,7);
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

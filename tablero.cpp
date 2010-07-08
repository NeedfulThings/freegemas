#include "tablero.h"


#include <ctime>
#include <iostream>
#include <boost/format.hpp>
using boost::format;

Tablero::Tablero(){
    lDEBUG << Log::CON("Tablero");
    generar();
}

void Tablero::generar(){
    std::srand(time(0));

    do{
	for (int i = 0; i < 8; ++i)
	{
	    for (int j = 0; j < 8; ++j)
	    {
		casillas[i][j] = static_cast<tCasilla>((int)Gosu::random(1,8));
	    }
	}
    }while(!comprobar().empty() || !existeSolucion()); // Regenera si hay alguna solución directa o si es imposible
    lDEBUG << "Generado un tablero con posibles soluciones, sin soluciones inmediatas";
}

void Tablero::swap(int x1, int y1, int x2, int y2){
    tCasilla temp = casillas[x1][y1];

    casillas[x1][y1] = casillas[x2][y2];
    casillas[x2][y2] = temp;
}

void Tablero::del(int x, int y){
    casillas[x][y] = casVacia;
}

bool Tablero::existeSolucion(){
    if(!comprobar().empty())
	return true;

    /* 
       Comprobaremos todos los posibles tableros
       (49 * 4) + (32 * 2) aunque hay muchos repetidos
    */
    bool flag = false;


    Tablero temp = *this;
    for(int x = 0; x < 8; ++x){
	for(int y = 0; y < 8; ++y){
	    
	    // Cambiar con superior y comprobar
	    if(y > 0){
		temp.swap(x,y, x,y-1);
		if(!temp.comprobar().empty()){
		    flag = true;
		}
		temp.swap(x,y, x,y-1);
	    }

	    // Cambiar con inferior y comprobar
	    if(y < 7){
		temp.swap(x, y, x, y+1);
		if(!temp.comprobar().empty()){
		    flag = true;
		}
		temp.swap(x, y, x, y+1);
	    }

	    // Cambiar con celda izquierda y comprobar
	    if(x > 0){
		temp.swap(x, y, x - 1, y);
		if(!temp.comprobar().empty()){
		    flag = true;
		}
		temp.swap(x, y, x - 1, y);
	    }

	    // Cambiar con celda derecha y comprobar
	    if(x < 7){
		temp.swap(x, y, x + 1, y);
		if(!temp.comprobar().empty()){
		    flag = true;
		}
		temp.swap(x, y, x + 1, y);
	    }
	}
    }
    
    
    return flag;

}

vector<coord> Tablero::comprobar(){
    // Recorremos filas
    tr1::array<tr1::array<int, 8>, 8> comprobHor;
    tr1::array<tr1::array<int, 8>, 8> comprobVer;

    // Ponemos matrices de comprobación a cero
    for(int y = 0; y < 8; ++y){
	for(int x = 0; x < 8; ++x){
	    comprobHor[x][y] = comprobVer[x][y] = 0;
	}
    }

    // Rellenamos la matriz de comprobaciones por FILAS
    for(int y = 0; y < 8; ++y){ 
	for(int x = 0; x < 8; ++x){ 

	    /* Recorremos de j - 1 hasta el inicio */
	    for(int k = x - 1; k >= 0; --k){ 
		if(casillas[x][y] == casillas[k][y] && casillas[x][y] != casVacia){
		    comprobHor[x][y] = comprobHor[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }

	    /* Recorremos de j + 1 hasta el final */
	    for(int k = x + 1; k < 8; ++k){
		if(casillas[x][y] == casillas[k][y] && casillas[x][y] != casVacia){
		    comprobHor[x][y] = comprobHor[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }
	}
    }


    // Rellenamos la matriz de comprobaciones por COLUMNAS
    for(int x = 0; x < 8; ++x){ 
	for(int y = 0; y < 8; ++y){ 

	    /* Recorremos de y - 1 hasta el inicio */
	    for(int k = y - 1; k >= 0; --k){ 
		if(casillas[x][y] == casillas[x][k] && casillas[x][y] != casVacia){
		    comprobVer[x][y] = comprobVer[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }

	    /* Recorremos de y + 1 hasta el final */
	    for(int k = y + 1; k < 8; ++k){
		if(casillas[x][y] == casillas[x][k] && casillas[x][y] != casVacia){
		    comprobVer[x][y] = comprobVer[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }
	}
    }


    bool flag = false;

    vector<coord> coordenadas;

    for(int y = 0; y < 8; ++y){ 
	for(int x = 0; x < 8; ++x){ 
	    if(comprobHor[x][y] > 1 || comprobVer[x][y] > 1){
		coordenadas.push_back(coord(x,y));
	    }
	}
    }

    return coordenadas;
}


void Tablero::calcularMovimientosCaida(){
    lDEBUG << "Tablero::calcularMovimientosCaida";
    for(int x = 0; x < 8; ++x){

	// De abajo a arriba
	for(int y = 7; y >= 0; --y){

	    // origY guarda la posición en el inicio de la caida
	    casillas[x][y].origY = y;
	    
	    // Si la casilla es vacía, bajamos todas las casillas por encima
	    if(casillas[x][y] == casVacia){
		lDEBUG << " columna " << x;
		lDEBUG << "  fila " << y;
		lDEBUG << format("   Casilla vacía: %i, %i") % x % y;

		for(int k = y-1; k >= 0; --k){		    
		    casillas[x][k].debeCaer = true;
		    casillas[x][k].destY ++;

		    lDEBUG << format("    origY: %i, destY: %i") % casillas[x][k].origY % casillas[x][k].destY;
		}
	    }
	}
    }
}

void Tablero::aplicarCaida(){
    lDEBUG << "Tablero::aplicarCaida";
    for(int x = 0; x < 8; ++x){
	// Desde abajo a arriba para no sobreescribir casillas

	for(int y = 7; y >= 0; --y){
	    if(casillas[x][y].debeCaer && casillas[x][y] != casVacia){
		int y0 = casillas[x][y].destY;
		
		lDEBUG << format(" - (%i,%i) debe caer %i casillas") % x % y % y0;

		casillas[x][y + y0] = casillas[x][y];
		casillas[x][y] = casVacia;
	    }
	}
   }
}

void Tablero::rellenarEspacios(){

    lDEBUG << "Tablero::rellenarEspacios";

	for(int x = 0; x < 8; ++x){
	    // Contar cuántos espacios hay que bajar
	    int saltos = 0;

	    for(int y = 0; y < 8; ++y){
		if(casillas[x][y] != casVacia) break;
		saltos ++;
	    }

	    // lDEBUG << format("%i saltos calculados para la columna %i") % saltos % x;

	    for(int y = 0; y < 8; ++y){
		if(casillas[x][y] == casVacia) {
		    
		    // lDEBUG << format("(%i,%i) está vacía, rellenar. OrigY: %i. Saltos: %i") % x % y % (y - saltos) % saltos;

		    casillas[x][y] = static_cast<tCasilla> ( (int)Gosu::random(1,8) );

		    
		    casillas[x][y].debeCaer = true;  
		    casillas[x][y].origY = y - saltos;
		    casillas[x][y].destY = saltos;
//*/

		}		
	    }
	}	
}


Tablero::~Tablero(){
    lDEBUG << Log::DES("Tablero");
}


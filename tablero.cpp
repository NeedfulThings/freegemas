#include "tablero.h"
#include "log.h"

#include <ctime>
#include <iostream>

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
    }while(comprobar());
}

tCasilla Tablero::get(int x, int y){
    return casillas[x][y];
}

tCasilla Tablero::swap(int x1, int y1, int x2, int y2){
    tCasilla temp = casillas[x1][y1];

    casillas[x1][y1] = casillas[x2][y2];
    casillas[x2][y2] = temp;
}

bool Tablero::existeSolucion(){
    if(comprobar())
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
		if(temp.comprobar()){
		    flag = true;
		}
		temp.swap(x,y, x,y-1);
	    }

	    // Cambiar con inferior y comprobar
	    if(y < 7){
		temp.swap(x, y, x, y+1);
		if(temp.comprobar()){
		    flag = true;
		}
		temp.swap(x, y, x, y+1);
	    }

	    // Cambiar con celda izquierda y comprobar
	    if(x > 0){
		temp.swap(x, y, x - 1, y);
		if(temp.comprobar()){
		    flag = true;
		}
		temp.swap(x, y, x - 1, y);
	    }

	    // Cambiar con celda derecha y comprobar
	    if(x < 7){
		temp.swap(x, y, x + 1, y);
		if(temp.comprobar()){
		    flag = true;
		}
		temp.swap(x, y, x + 1, y);
	    }
	}
    }
    
    
    return flag;

}

bool comprobarFila(int y){

}

bool comprobarColumna(int i){

}


bool Tablero::comprobar(){
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
		if(casillas[x][y] == casillas[k][y]){
		    comprobHor[x][y] = comprobHor[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }

	    /* Recorremos de j + 1 hasta el final */
	    for(int k = x + 1; k < 8; ++k){
		if(casillas[x][y] == casillas[k][y]){
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
		if(casillas[x][y] == casillas[x][k]){
		    comprobVer[x][y] = comprobVer[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }

	    /* Recorremos de y + 1 hasta el final */
	    for(int k = y + 1; k < 8; ++k){
		if(casillas[x][y] == casillas[x][k]){
		    comprobVer[x][y] = comprobVer[x][y] + 1;
		}else{
		    break; // Si alguna casilla no coincide, pues pasamos
		}
	    }
	}
    }


    bool flag = false;

    for(int y = 0; y < 8; ++y){ 
	for(int x = 0; x < 8; ++x){ 
	    if(comprobHor[x][y] > 1 || comprobVer[x][y] > 1){
		flag = true;
	    }
	}
    }

    return flag;
}

Tablero::~Tablero(){
    lDEBUG << Log::DES("Tablero");
}


#include "juego.h"
#include "log.h"
#include "estado.h"

#include "estadoJuego.h"


Juego::Juego () : Gosu::Window(800, 600, false) {
    lDEBUG << Log::CON("Juego");
    setCaption(L"FreeGemas v0.1");

    puntero.reset(new Gosu::Image(graphics(), L"media/handCursor.png"));
    selector.reset(new Gosu::Image(graphics(), L"media/selector.png"));
    cambiarEstado("estadoJuego");
}

void Juego::update(){
    estadoActual -> update();
}

void Juego::draw(){
    puntero -> draw(input().mouseX(), input().mouseY(), 90);
    estadoActual -> draw();
}

void Juego::buttonDown(Gosu::Button boton){
    estadoActual -> buttonDown(boton);
}

void Juego::cambiarEstado(string S){
    if(S == estadoActualString)
	return;

    if(S == "estadoJuego"){
	estadoActual . reset(new EstadoJuego(this));
	estadoActualString = "estadoJuego";
    }
}

Juego::~Juego(){
    lDEBUG << Log::DES("Juego");
}

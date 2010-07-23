#include "stateHowtoplay.h"

#include "game.h"
#include "resManager.h"
#include "inter.h"

StateHowtoplay::StateHowtoplay(Game * p) : State(p){
    imgBackground = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/howtoScreen.png");

    fontTitle = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fuenteMenu.ttf", 48);
    fontSubtitle = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fuenteMenu.ttf", 23);

    wstring bodyText;
    bodyText += Gosu::utf8ToWstring(_("El objetivo del juego es formar grupos de tres o más gemas iguales, en horizontal o vertical, intercambiando gemas adyacentes."));
    bodyText += Gosu::utf8ToWstring(_("\n\n"));
    bodyText += Gosu::utf8ToWstring(_("Haz click en la primera gema y luego haz click en la gema por la que la quieras intercambiar. si el movimiento es correcto, se intercambiarán y las gemas que formen grupos desaparecerán, dándote puntos."));
    bodyText += Gosu::utf8ToWstring(_("\n\n"));
    bodyText += Gosu::utf8ToWstring(_("Puedes intentar hacer grupos de cuatro o más gemas para conseguir más puntos, y pensar en estrategias para provocar reacciones en cadena."));

    Gosu::Bitmap B = Gosu::createText(bodyText, 
				      Gosu::resourcePrefix() + L"media/fNormal.ttf", 
				      28,
				      6, 
				      470,
				      Gosu::taCenter);
    imgBodyText.reset(new Gosu::Image(parent -> graphics(), B));
}

void StateHowtoplay::update(){

}

void StateHowtoplay::draw(){
    imgBackground -> draw(0,0,0);

    wstring titleText = Gosu::utf8ToWstring(_("Cómo jugar"));
    wstring exitText = Gosu::utf8ToWstring(_("Pulsa escape para volver"));

    fontTitle -> draw(titleText, 
		      300 + 470 / 2 - fontTitle -> textWidth(titleText) / 2,
		      20, 1);

    fontTitle -> draw(titleText, 
		      300 + 470 / 2 - fontTitle -> textWidth(titleText) / 2 + 1,
		      20 + 2, 0.9,
		      1, 1, 0x44000000);

    fontSubtitle -> draw(exitText,
			 30, 550, 1);

    fontSubtitle -> draw(exitText,
			 30 + 1, 550 + 2, 0.9,
			 1, 1, 0x44000000);

    imgBodyText -> draw(300, 110, 1);

    imgBodyText -> draw(300+1, 110+2, 0.9, 1, 1, 0x44000000);
}

void StateHowtoplay::buttonDown(Gosu::Button B){
    if(B == Gosu::kbEscape){
	parent -> changeState("stateMainMenu");
    }
}

StateHowtoplay::~StateHowtoplay(){

}

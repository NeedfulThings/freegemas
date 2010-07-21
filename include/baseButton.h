#ifndef _BASEBUTTON_
#define _BASEBUTTON_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>

#include <string>

#include "resManager.h"

class BaseButton{
public:
    BaseButton(Gosu::Graphics & g, std::wstring text, std::wstring imgIconPath = L"") : g(g){
	buttonBackground = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/buttonBackground.png");

	if(imgIconPath != L""){
	    imgIcon = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/" + imgIconPath);	
	}

	changeText(text);
    }

    void changeText(std::wstring text){
	Gosu::Bitmap B = Gosu::createText(text, Gosu::resourcePrefix() + L"media/fNormal.ttf", 28, 0, 
					  buttonBackground -> width() - ((imgIcon != 0) ? 40 : 0), 
					  Gosu::taCenter, 0);
	buttonText.reset(new Gosu::Image(g, B));
    }

    void draw(int x, int y, double z){
	lastX = x;
	lastY = y;

	if(imgIcon != 0){
	    imgIcon -> draw(x + 7, y, z + 0.1);
	    buttonText -> draw(x + 40, y + 5, z + 0.1);
	}else{
	    buttonText -> draw(x, y + 5, z + 0.1);
	}
	buttonBackground -> draw(x, y, z);
	
    }

    bool clicked(unsigned int mX, unsigned int mY){
	if(mX > lastX && mX < lastX + buttonBackground -> width() &&
	   mY > lastY && mY < lastY + buttonBackground -> height()){
	    return true;
	}else{
	    return false;
	}
    }

private:
    boost::shared_ptr<Gosu::Image> buttonBackground;
    boost::shared_ptr<Gosu::Image> buttonText;
    boost::shared_ptr<Gosu::Image> imgIcon;

    Gosu::Graphics & g;

    unsigned int lastX, lastY;
};

#endif

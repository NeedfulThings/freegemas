#ifndef _BASEBUTTON_
#define _BASEBUTTON_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>

#include <string>

class BaseButton{
public:
    BaseButton(Gosu::Graphics & g, std::wstring text) : g(g){
	buttonBackground.reset(new Gosu::Image(g, Gosu::resourcePrefix() + L"media/buttonBackground.png"));
		
	changeText(text);
    }

    void changeText(std::wstring text){
	Gosu::Bitmap B = Gosu::createText(text, Gosu::resourcePrefix() + L"media/fNormal.ttf", 28, 0, buttonBackground -> width(), Gosu::taCenter, 0);
	buttonText.reset(new Gosu::Image(g, B));
    }

    void draw(int x, int y, double z){
	lastX = x;
	lastY = y;
	buttonBackground -> draw(x, y, z);
	buttonText -> draw(x, y + 5, z + 0.1);
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
    boost::scoped_ptr<Gosu::Image> buttonBackground;
    boost::scoped_ptr<Gosu::Image> buttonText;

    Gosu::Graphics & g;

    unsigned int lastX, lastY;
};

#endif

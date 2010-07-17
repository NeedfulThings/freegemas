#ifndef _BASEBUTTON_
#define _BASEBUTTON_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>

#include <string>

class BaseButton{
public:
	BaseButton(Gosu::Graphics & g, std::wstring text){
		buttonBackground.reset(new Gosu::Image(g, Gosu::resourcePrefix() + L"media/buttonBackground.png"));
		
		Gosu::Bitmap B = Gosu::createText(text, Gosu::resourcePrefix() + L"media/fNormal.ttf", 28, 0, buttonBackground -> width(), Gosu::taCenter, 0);
		buttonText.reset(new Gosu::Image(g, B));
	}

	void draw(int x, int y, double z){
		buttonBackground -> draw(x, y, z);
		buttonText -> draw(x, y + 6, z + 0.1);
	}
private:
	boost::scoped_ptr<Gosu::Image> buttonBackground;
	boost::scoped_ptr<Gosu::Image> buttonText;
};

#endif
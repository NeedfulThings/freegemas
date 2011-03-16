#ifndef _BASEBUTTON_
#define _BASEBUTTON_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>

#include <string>
using std::wstring;

#include "resManager.h"
#include "log.h"

class BaseButton{
public:
    BaseButton(Gosu::Graphics & g, std::wstring text, std::wstring imgIconPath = L"") : g(g){
        buttonBackground = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/buttonBackground.png");
        buttonFont = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fNormal.ttf", 29);

        if(imgIconPath != L""){
            imgIcon = ResMgr -> getImage(Gosu::resourcePrefix() + L"media/" + imgIconPath);	
        }

        changeText(text);
    }

    void changeText(std::wstring text){
        buttonText = text;
        int text_w = buttonFont -> textWidth(buttonText) / 2;

        if(imgIcon != 0){
            int espacio = 35;
            textX = Gosu::round(espacio + (buttonBackground -> width() - espacio) / 2 - text_w);
        }else{
            textX = Gosu::round(buttonBackground -> width() / 2 - text_w);
        }
    }

    void draw(int x, int y, double z){
        lastX = x;
        lastY = y;

        if(imgIcon != 0){
            imgIcon -> draw(x + 7, y, z + 0.1);
        }

        buttonFont -> draw(buttonText,
                           x + textX, y + 5, z + 0.2);

        buttonFont -> draw(buttonText,
                           x + textX + 1, y + 7, z + 0.1,
                           1,1,0x44000000);

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
    boost::shared_ptr<Gosu::Image> imgIcon;

    wstring buttonText;
    int textX;

    boost::shared_ptr<Gosu::Font> buttonFont;
    
    Gosu::Graphics & g;

    unsigned int lastX, lastY;
};

#endif

#ifndef _FLOATINGSCORE_H_
#define _FLOATINGSCORE_H_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
using namespace std;

#include "log.h"

class FloatingScore{
public:
    FloatingScore(Gosu::Graphics & g, int score, int x, int y) : 
	scoreText(boost::lexical_cast<string>(score)), x_(x), y_(y), currentStep(0), totalSteps(100){

	// lDEBUG << Log::CON("FloatingScore") << " " << x << "," << y;

	Gosu::Bitmap B = Gosu::createText(Gosu::widen(scoreText),
					  L"media/fuentelcd.ttf", 60);
	scoreImage.reset(new Gosu::Image(g, B));
    }

    bool ended(){
	return currentStep == totalSteps;
    }

    void draw(){
	if(currentStep != totalSteps) ++currentStep;

	++currentStep;
	float p = 1. - (float)currentStep/totalSteps;

	int posX = 241 + x_ * 65;
	int posY = 41 + y_ * 65 - (1 - p) * 20; 

	scoreImage -> draw(posX, 
			   posY, 
			   6, 1, 1,
			   Gosu::Color(p * 255, 255, 255, 255));

	scoreImage -> draw(posX + 4,
			   posY + 4,
			   5.9, 1, 1,
			   Gosu::Color(p * 255, 0, 0, 0));

    }
private:
    boost::shared_ptr<Gosu::Image> scoreImage;
    
    string scoreText;
			   
    int x_;
    int y_;
    
    int currentStep;
    int totalSteps;
			   

};

#endif /* _FLOATINGSCORE_H_ */

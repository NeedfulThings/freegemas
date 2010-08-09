#ifndef _JEWELGROUPANIM_H_
#define _JEWELGROUPANIM_H_

#include <Gosu/Gosu.hpp>

#include <boost/shared_ptr.hpp>

#include "resManager.h"

class JewelGroupAnim{
public:
    JewelGroupAnim(){
	imgGems[0] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemWhite.png");
	imgGems[1] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemRed.png");
	imgGems[2] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemPurple.png");
	imgGems[3] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemOrange.png");
	imgGems[4] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemGreen.png");
	imgGems[5] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemYellow.png");
	imgGems[6] = ResMgr -> getImage (Gosu::resourcePrefix() + L"media/gemBlue.png");

	for (int i = 0; i < 7; ++i)
	{
	    posX[i] = 800 / 2 - (65 * 7) / 2 + i * 65;
	}

	animationCurrentStep = 0;
	animationTotalSteps = 30;
	posFinalY = 265;
    }

    void draw(){
	if(animationCurrentStep < 7 * 5 + animationTotalSteps)
	    ++animationCurrentStep;

	for(int i = 0; i < 7; ++i)
	{
	    int composedStep = animationCurrentStep - i * 5;
	    if(composedStep < 0) continue;

	    if(composedStep < animationTotalSteps){
		imgGems[i] -> draw(posX[i], 
				   eqMovOut((float) composedStep, 600.0, (float) posFinalY - 600., (float) animationTotalSteps), 
				   2.);
	    }else{
		imgGems[i] -> draw(posX[i], posFinalY, 2);
	    }
	}
    }

    // past time, begining pos, change, duration
    float eqMovOut(float t, float b, float c, float d) {
	/* //quad
	t/=d;
	return -c *(t)*(t-2) + b;
	//*/

	// cubic
	t=t/d-1;
	return c*(t*t*t + 1) + b;
    };
    
private:
    boost::shared_ptr<Gosu::Image> imgGems[7];

    int posX[7], posFinalY;
    
    int animationCurrentStep;
    int animationTotalSteps;

};


#endif /* _JEWELGROUPANIM_H_ */

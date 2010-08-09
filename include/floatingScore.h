/**
 * @file floatingScore.h
 * 
 * @author José Tomás Tocino García
 * @date 2010
 *
 * Copyright (C) 2010 José Tomás Tocino García <theom3ga@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


#ifndef _FLOATINGSCORE_H_
#define _FLOATINGSCORE_H_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
using namespace std;

#include "log.h"
#include "resManager.h"

class FloatingScore{
public:
    FloatingScore(Gosu::Graphics & g, int score, int x, int y) : 
	scoreText(boost::lexical_cast<wstring>(score)), 
	x_(x), y_(y), currentStep(0), totalSteps(100){

	scoreFont = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fuentelcd.ttf", 60);
    }

    bool ended(){
	return currentStep == totalSteps;
    }

    void draw(){
	if(currentStep != totalSteps) ++currentStep;

	++currentStep;
	float p = 1. - (float)currentStep/totalSteps;

	int posX = 241 + x_ * 65;
	int posY = int(41 + y_ * 65 - (1 - p) * 20); 

	scoreFont -> draw(scoreText,
			  posX, 
			  posY, 
			  6, 1, 1,
			  Gosu::Color((int)(p * 255), 255, 255, 255));

	scoreFont -> draw(scoreText,
			   posX + 4,
			   posY + 4,
			   5.9, 1, 1,
			   Gosu::Color((int)(p * 255), 0, 0, 0));

    }
private:
    boost::shared_ptr<Gosu::Image> scoreImage;

    boost::shared_ptr<SDLFont> scoreFont;
    wstring scoreText;
			   
    int x_;
    int y_;
    
    int currentStep;
    int totalSteps;
			   

};

#endif /* _FLOATINGSCORE_H_ */

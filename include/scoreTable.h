/**
 * @file scoreTable.h
 * 
 * @author José Tomás Tocino García
 * @date 2010
 *
 * 
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


#ifndef _SCORETABLE_
#define _SCORETABLE_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

#include "log.h"
#include "game.h"
#include "resManager.h"

class ScoreTableInput : public Gosu::TextInput{
public:
    std::wstring filter(const std::wstring & str_) const{
	if(text().length() == 15) 
	    return L"";

	lDEBUG << "Filter: " << Gosu::narrow(str_);
	std::wstring returnString = str_.substr(0, 15);
	returnString.erase(std::remove_if(returnString.begin(), returnString.end(), 
					  boost::bind<bool>(std::equal_to<wchar_t>(), _1, L' ')), 
			   returnString.end());

	return returnString;
    }

};

class Game;

class ScoreTable{
public:
    ScoreTable(Game * p, int points);

    void draw(int x, int y, double z);
    void buttonDown(Gosu::Button B);

private:

    void fillEmptyScoreFile();

    boost::shared_ptr<Gosu::Font> fntH1, fntH2;

    boost::shared_ptr<Gosu::Font> fntLcdBig, fntLcdSmall;
    fstream scoreFile;

    enum tState{
	eRequestPlayerName,
	eShowScores
    };

    tState state;

    struct scoreComp{
	bool operator()(const pair<string,int> & A, const pair<string,int>& B){
	    return A.second > B.second;
	}
    };

    set<pair<string, int>, scoreComp > readScoreSet;
    typedef set<pair<string, int>, scoreComp >::iterator scoreSetIterator;
    int scoreBoardWidth;

    Game * parent;
    ScoreTableInput nameInput;
    
    int points;
};

#endif

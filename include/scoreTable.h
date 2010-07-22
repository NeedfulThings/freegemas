#ifndef _SCORETABLE_
#define _SCORETABLE_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>

#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

#include "log.h"
#include "game.h"


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

private:
    boost::shared_ptr<Gosu::Font> fntH1, fntH2;
    fstream scoreFile;

    enum tState{
	eRequestPlayerName
    };

    tState state;

    struct scoreComp{
	bool operator()(const pair<string,int> & A, const pair<string,int>& B){
	    return A.second < B.second;
	}
    };

    set<pair<string, int>, scoreComp > readScoreSet;
    int scoreBoardWidth;

    Game * parent;
    ScoreTableInput nameInput;
};

#endif

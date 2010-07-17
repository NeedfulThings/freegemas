#ifndef _SCORETABLE_
#define _SCORETABLE_

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
class ScoreTable{
public:
	ScoreTable(Gosu::Graphics & g, int points){
		fntMain.reset(new Gosu::Font(g, Gosu::resourcePrefix() + L"media/fuenteMenu.ttf", 30, 0));
	}

	void draw(int x, int y, double z){
		fntMain -> draw(L"Game Over, loser", x, y, z);
	}

private:
	boost::scoped_ptr<Gosu::Font> fntMain;
};

#endif
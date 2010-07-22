#include "scoreTable.h"
#include "resManager.h"


ScoreTable::ScoreTable(Game * p, int points) : parent(p){
    fntH1 = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fuenteMenu.ttf", 50);
    fntH2 = ResMgr -> getFont(Gosu::resourcePrefix() + L"media/fNormal.ttf", 28);

    string scoreFilePath = Gosu::narrow(Gosu::userSettingsPrefix()) + "freeGemasScore"; 
    lDEBUG << "Reading file " << scoreFilePath;
    scoreFile.open(scoreFilePath.c_str(), fstream::in);
    
    while(!scoreFile.eof() && scoreFile.good()){
	pair<string, int> currentScore;
	scoreFile >> currentScore.first;
	scoreFile >> currentScore.second;
	lDEBUG << "Read: " << currentScore.first << " => " << currentScore.second;
	readScoreSet.insert(currentScore);
    }
    
    state = eRequestPlayerName;
    scoreBoardWidth = 300;

    parent -> input().setTextInput(&nameInput);
    
}

#define KOL 0xffffffff

void ScoreTable::draw(int x, int y, double z){
    wstring stringTitle = L"FIN DEL JUEGO",
	stringSubtitle = L"Escribe tu nombre:";
    
    int w1 = fntH1 -> textWidth(stringTitle);
    int w2 = fntH2 -> textWidth(stringSubtitle);

    float center = x + scoreBoardWidth / 2;
    fntH1 -> draw(stringTitle, 
		 center - w1 / 2, 
		  y, z);

    fntH1 -> draw(stringTitle, 
		 center - w1 / 2 + 1, 
		  y+3, z - 0.1, 1, 1, 0x44000000);

    fntH2 -> draw(stringSubtitle, 
		 center - w2 / 2, 
		  y + 80, z); 

    parent -> graphics().drawLine(x, y + 150, KOL,
				  x + scoreBoardWidth, y + 150, KOL, z);

    fntH2 -> draw(nameInput.text(),
		  center - fntH2 -> textWidth(nameInput.text()) / 2, 
		  y + 150 - 33, z);
}

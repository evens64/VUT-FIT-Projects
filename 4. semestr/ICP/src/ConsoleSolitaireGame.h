// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef CONSOLESOLITAIREGAME_H_INCLUDED
#define CONSOLESOLITAIREGAME_H_INCLUDED

#include <iostream>
#include "ConsoleSolitaireBoard.h"
class ConsoleSolitaireGame
{
public:
	ConsoleSolitaireGame();
	~ConsoleSolitaireGame();

	void writeoutAllBoards() const;

	void mainLoop();

	bool executeCommand();

	void printHelp();
	void writeInvalidCommand();

protected:
	ConsoleSolitaireBoard mBoards[4];
	bool mBoardsActive[4] = {false, false, false, false};
};

#endif

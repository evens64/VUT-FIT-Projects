// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef CONSOLESOLITAIREBOARD_H_INCLUDED
#define CONSOLESOLITAIREBOARD_H_INCLUDED

#include <iostream>
#include <map>
#include <string>
#include "SolitaireBoard.h"
#include "CardStack.h"
/**
* \brief Class representing a playing board, enriched for methods related to console output and input
*/
class ConsoleSolitaireBoard : public SolitaireBoard
{
public:
	

	ConsoleSolitaireBoard();
	~ConsoleSolitaireBoard();

	void writeoutGameStatus() const;
	void writeoutStackGroup(const CardStackGroups group) const;
	void writeoutStack(const CardStackGroups group, const int index) const;
	void writeoutHints(int limit) const;
};

#endif

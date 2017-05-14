// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef CONSOLESOLITAIREBOARD_H_INCLUDED
#define CONSOLESOLITAIREBOARD_H_INCLUDED

#include <iostream>
#include <map>
#include <string>
#include "SolitaireBoard.h"
#include "CardStack.h"

class ConsoleSolitaireBoard : public SolitaireBoard
{
public:
	const std::map<Card::CardValues, std::string> value2str = {
		{ Card::ace, "A" },
		{ Card::two, "2" },
		{ Card::three, "3" },
		{ Card::four, "4" },
		{ Card::five, "5" },
		{ Card::six, "6" },
		{ Card::seven, "7" },
		{ Card::eight, "8" },
		{ Card::nine, "9" },
		{ Card::ten, "10" },
		{ Card::jack, "J" },
		{ Card::queen, "Q" },
		{ Card::king, "K" }
	};
	const std::map<Card::CardSuits, char> suit2char = {
		{ Card::diamonds, 'D' },
		{ Card::hearts, 'H' },
		{ Card::clubs, 'C' },
		{ Card::spades, 'S' },
	};
	const std::map<CardStackGroups, char> group2char = {
		{ deckGroup, 'D' },
		{ drawGroup, 'W' },
		{ pileGroup, 'P' },
		{ foundationGroup, 'F' },
	};

	ConsoleSolitaireBoard();
	~ConsoleSolitaireBoard();

	void writeoutGameStatus() const;
	void writeoutStackGroup(const CardStackGroups group) const;
	void writeoutStack(const CardStackGroups group, const int index) const;
};

#endif

// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef SOLITAIREBOARD_H_INCLUDED
#define SOLITAIREBOARD_H_INCLUDED

#include <vector>
#include <array>
#include <map>
#include <fstream>
#include <string>
#include "CardStack.h"


/**
* \brief Struct for recording moves in the game
*/
class SolitaireGameMove;
/**
* \brief Class for containing solitaire games
*/
class SolitaireBoard
{
public:
	/**
	* \brief Enumerator for Groups of stacks
	*/
	enum CardStackGroups {
		deckGroup, /**< Group containing deck only */
		drawGroup, /**< Group containing stacks for drawn cards */
		pileGroup, /**< Group containing pile stacks */
		foundationGroup, /**< Group containing foundation stacks */
		sizeOfCardStackGroups /**< utility element */
	};

	/**
	* \brief number of stacks in each group
	*
	* number of stacks in each group, keep consistent with CardStackGroups
	*/
	const int GroupSizes[sizeOfCardStackGroups] = { 1,1,7,4 };
	const CardStack CardStackPrototypes[4] = {
		CardStack(CardStack::stackNotVisible,CardStack::anySuit,CardStack::noOrder,false,false),
		CardStack(CardStack::stackOnlyTopVisible,CardStack::anySuit,CardStack::noOrder,true,false),
		CardStack(CardStack::stackTopMadeVisible,CardStack::alternating,CardStack::descendingOrder,true,true),
		CardStack(CardStack::stackOnlyTopVisible,CardStack::hearts,CardStack::ascendingOrder,true,false)
	};
	/**
	* \brief Enumerator for type of performed moves
	*/
	enum MoveTypes {
		moveMove, /**< normal move */
		drawMove, /**< draw of card from deck */
		recycleMove, /**< recycling stack of drawn cards */
		MoveTypeslast = recycleMove
	};

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
	/*
	const std::map<char, CardStackGroups> char2group = {
		{ 'D', deckGroup },
		{ 'W', drawGroup },
		{ 'P', pileGroup },
		{ 'F', foundationGroup },
	};
	*/
	const std::map<std::string, Card::CardValues> str2value = {
		{ "A", Card::ace },
		{ "2", Card::two },
		{ "3", Card::three },
		{ "4", Card::four },
		{ "5", Card::five },
		{ "6", Card::six },
		{ "7", Card::seven },
		{ "8", Card::eight },
		{ "9", Card::nine },
		{ "10", Card::ten },
		{ "J", Card::jack },
		{ "Q", Card::queen },
		{ "K", Card::king }
	};
	const std::map<char, Card::CardSuits> char2suit = {
		{ 'D', Card::diamonds },
		{ 'H', Card::hearts },
		{ 'C', Card::clubs },
		{ 'S', Card::spades },
	};

//protected:

	std::vector<SolitaireGameMove> mLastMoves;
	int mScore = 0;
	std::array<std::vector<CardStack>, sizeOfCardStackGroups> mStacks = { {
		std::vector<CardStack>(GroupSizes[deckGroup], CardStackPrototypes[deckGroup]),
		std::vector<CardStack>(GroupSizes[drawGroup], CardStackPrototypes[drawGroup]),
		std::vector<CardStack>(GroupSizes[pileGroup], CardStackPrototypes[pileGroup]),
		std::vector<CardStack>(GroupSizes[foundationGroup], CardStackPrototypes[foundationGroup])
	} };

//public:
	SolitaireBoard();
	~SolitaireBoard();
	bool move(CardStackGroups sourceGroup, int sourceIndex, CardStackGroups targetGroup, int targetIndex, int amount);
	bool move(SolitaireGameMove & move);
	bool undo();
	void draw();
	void initialDeal();
	void recycleDrawn();
	std::vector<SolitaireGameMove> getHints(unsigned int limit) const;

	const CardStack& getStackRef(CardStackGroups group, int index) const;
	int getScore() const;

	void save(char* filename);
	bool load(char* filename);

	void clearBoard();
};

typedef std::pair<SolitaireBoard::CardStackGroups, int> StackID;

/**
* \brief class for storing information about game moves
*/
class SolitaireGameMove {
public:
	enum MoveTypes {
		move,
		draw,
		recycle,
	};

	SolitaireGameMove(
		MoveTypes moveType,
		SolitaireBoard::CardStackGroups sourceGroup,
		int mSourceIndex,
		SolitaireBoard::CardStackGroups targetGroup,
		int targetIndex,
		int cardAmount,
		bool priorVisibility,
		bool priorVisibilityAltered
	);

	SolitaireGameMove();

	MoveTypes mMoveType;
	SolitaireBoard::CardStackGroups mSourceGroup; /**< group and stack from which the cards were taken */
	int mSourceIndex; /**< group and stack to which the cards are moved*/
	SolitaireBoard::CardStackGroups mTargetGroup;
	int mTargetIndex;
	int mCardAmount; /**< number of cards transferred */
	bool mPriorVisibility;
	bool mPriorVisibilityAltered;
	int mScoreChange = 0;
};

#endif

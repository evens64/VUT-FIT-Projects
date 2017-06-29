// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)
#include "ConsoleSolitaireBoard.h"


/**
* \brief A constructor
*/
ConsoleSolitaireBoard::ConsoleSolitaireBoard()
{
}

/**
* \brief A trivial destructor
*
* does nothing, really
*/
ConsoleSolitaireBoard::~ConsoleSolitaireBoard()
{
}


/**
* \brief Writes out status of a board into standard output
*
* Writes out status of the board into standard output./
* calls writeoutStackGroup, for each group, writes out score,
*
* Format: number of cards in deck, number of cards in waste, line of -----
* group sections separeted by lines of -------, line of --------, score
*/
void ConsoleSolitaireBoard::writeoutGameStatus() const
{
	std::cout << "number of cards in deck: " << this->mStacks[deckGroup][0].getSize() << std::endl;
	std::cout << "number of cards in draw: " << this->mStacks[drawGroup][0].getSize() << std::endl;
	for (unsigned int i = 1; i < this->mStacks.size(); i++) {
			std::cout << "---------------------------------------------" << std::endl;
			this->writeoutStackGroup(static_cast<CardStackGroups>(i));
	}
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "score: " << this->mScore << std::endl;
}

/**
* \brief Writes out status of a stackGroup
*
* Writes out status of a stackGroup
* Format: Stack sections separated by newlines
*
*/
void ConsoleSolitaireBoard::writeoutStackGroup(const CardStackGroups group) const
{
	for (unsigned int i = 0; i < this->mStacks[group].size(); i++) {
		writeoutStack(group, i);
		std::cout << std::endl;
	}
}

/**
* \brief Writes out status of particular stack of cards
*
* Writes out status status of particular stack of cards
* Format: Letter of stackgroup ('W'aste, 'P'ile, 'F'oundation), ID of stack, :, Cards separated by spaces
* Format of cards is letter of suit ('H'earts, 'D'iamonds, 'C'lubs, 'S'pades) and value of card, invisible cards are displayed as **
* example: P5: ** ** ** HJ C10 D9
*/
void ConsoleSolitaireBoard::writeoutStack(const CardStackGroups group, const int index) const
{
	switch (this->mStacks[group][index].getVisibility()) {
	case CardStack::stackNotVisible:
		return;
	case CardStack::stackOnlyTopVisible:
		std::cout << this->group2char.at(group) << std::to_string(index);
		if (group == CardStackGroups::foundationGroup) std::cout << this->suit2char.at(static_cast<Card::CardSuits>(this->mStacks[group][index].getSuitRule()));
		std::cout << ":";
		if (this->mStacks[group][index].getSize() == 0) return;
		std::cout << " " << this->suit2char.at((*this->mStacks[group][index].begin()).getSuit()) << this->value2str.at((*this->mStacks[group][index].begin()).getValue());
		return;
	default:
		std::cout << this->group2char.at(group) << std::to_string(index) << ":";
		if (this->mStacks[group][index].getSize() == 0) return;
		for (auto iter = this->mStacks[group][index].rbegin(); iter != this->mStacks[group][index].rend(); ++iter) {
			if ((*iter).getVisible() == false) {
				std::cout << " **"; 
			}
			else {
				auto suit = this->suit2char.at((*iter).getSuit());
				auto value = this->value2str.at((*iter).getValue());
				std::cout << " " << suit << value;
			}
		}
		return;
	}
}
/**
* \brief Writes out Hints for the game
* \param[in] limit maximum number of hints to write out
*/
void ConsoleSolitaireBoard::writeoutHints(int limit) const
{
	auto hints = this->getHints(limit);
	if (hints.size() > 0) {
		for (auto iter = hints.begin(); iter != hints.end(); ++iter) {
			switch ((*iter).mMoveType) {
			case SolitaireGameMove::move:
				std::cout << "move " << (*iter).mCardAmount << " card(s) from " << group2char.at((*iter).mSourceGroup) << (*iter).mSourceIndex << " to " << group2char.at((*iter).mTargetGroup) << (*iter).mTargetIndex << std::endl;
				break;
			case SolitaireGameMove::draw:
				std::cout << "draw a card" << std::endl;
				break;
			case SolitaireGameMove::recycle:
				std::cout << "recycle waste (possibly out of moves)" << std::endl;
			}
		}
	}
	else std::cout << "no possible moves left" << std::endl;

}


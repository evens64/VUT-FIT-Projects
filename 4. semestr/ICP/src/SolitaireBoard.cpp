// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)
#include "SolitaireBoard.h"


/**
* \brief A constructor
*
* initialises cardstacks of the game
*/
SolitaireBoard::SolitaireBoard()
{
	this->mStacks[foundationGroup][1].setSuitRule(CardStack::diamonds);
	this->mStacks[foundationGroup][2].setSuitRule(CardStack::clubs);
	this->mStacks[foundationGroup][3].setSuitRule(CardStack::spades);
}

/**
* \brief A destructor
*/
SolitaireBoard::~SolitaireBoard()
{
}

/**
* \brief Attempts to move card from one stack to another
* \param[in] sourceGroup Group of stacks from which the cards are moved
* \param[in] sourceIndex Index of Stack within Group specified by sourceGroup from which the cards are moved
* \param[in] targetGroup Group of stacks to which the cards are moved
* \param[in] targetIndex Index of Stack within Group specified by targetGroup to which the cards are moved
* \param[in] amount number of cards to be moved
* \return True if move is successful, False if it is invalid
*
* Attempts to move card from one stack to another, with respect to thir rules,
* if move is successful, updates score, and adds record to mLastMoves (maximum number of moves remembered optional)
*/
bool SolitaireBoard::move(CardStackGroups sourceGroup, int sourceIndex, CardStackGroups targetGroup, int targetIndex, int amount)
{
	SolitaireGameMove newmove = SolitaireGameMove(SolitaireGameMove::move, sourceGroup, sourceIndex, targetGroup, targetIndex, amount, false, false);

	if (this->mStacks[sourceGroup][sourceIndex].getSize() > amount) {
		auto iter = this->mStacks[sourceGroup][sourceIndex].begin();
		for (int i = 0; i < amount; i++ ) ++iter;
		const Card& priorCard = *iter;
		newmove.mPriorVisibility = priorCard.getVisible();
		if (!this->mStacks[sourceGroup][sourceIndex].moveCard(mStacks[targetGroup][targetIndex], amount)) return false;
		newmove.mPriorVisibilityAltered = newmove.mPriorVisibility != priorCard.getVisible();
	}
	else {
		if (!this->mStacks[sourceGroup][sourceIndex].moveCard(mStacks[targetGroup][targetIndex], amount)) return false;
		newmove.mPriorVisibility = false;
		newmove.mPriorVisibilityAltered = false;
	}

	if (sourceGroup == CardStackGroups::drawGroup && targetGroup == CardStackGroups::pileGroup) newmove.mScoreChange = 5;
	else if (sourceGroup == CardStackGroups::drawGroup && targetGroup == CardStackGroups::foundationGroup) newmove.mScoreChange = 10;
	else if (sourceGroup == CardStackGroups::foundationGroup && targetGroup == CardStackGroups::pileGroup) newmove.mScoreChange = -15;
	else if (sourceGroup == CardStackGroups::pileGroup && targetGroup == CardStackGroups::foundationGroup) newmove.mScoreChange = 10;
	if (sourceGroup == CardStackGroups::pileGroup && newmove.mPriorVisibility == false && newmove.mPriorVisibilityAltered == true) newmove.mScoreChange += 5;
	if (this->mScore < -newmove.mScoreChange) newmove.mScoreChange = -this->mScore;
	this->mScore += newmove.mScoreChange;

	this->mLastMoves.push_back(newmove);
	return true;
}

/**
* \brief Attempts to move card from one stack to another
* \param[in] move move to execute
* \return True if move is successful, False if it is invalid
*
* Attempts to move card from one stack to another, with respect to thir rules,
* if move is successful, updates score, and adds record to mLastMoves (maximum number of moves remembered optional)
*/
bool SolitaireBoard::move(SolitaireGameMove& move)
{
	if (this->mStacks[move.mSourceGroup][move.mSourceIndex].getSize() > move.mCardAmount){
		auto iter = this->mStacks[move.mSourceGroup][move.mSourceIndex].begin();
		for (int i = 0; i < move.mCardAmount; i++) ++iter;
		const Card& priorCard = *iter;
		move.mPriorVisibility = priorCard.getVisible();
		if (!this->mStacks[move.mSourceGroup][move.mSourceIndex].moveCard(mStacks[move.mTargetGroup][move.mTargetIndex], move.mCardAmount)) return false;
		move.mPriorVisibilityAltered = move.mPriorVisibility != priorCard.getVisible();
	}
	else {
		if (!this->mStacks[move.mSourceGroup][move.mSourceIndex].moveCard(mStacks[move.mTargetGroup][move.mTargetIndex], move.mCardAmount)) return false;
		move.mPriorVisibility = false;
		move.mPriorVisibilityAltered = false;
	}

	move.mMoveType = SolitaireGameMove::move;

	if (move.mSourceGroup == CardStackGroups::drawGroup && move.mTargetGroup == CardStackGroups::pileGroup) move.mScoreChange = 5;
	else if (move.mSourceGroup == CardStackGroups::drawGroup && move.mTargetGroup == CardStackGroups::foundationGroup) move.mScoreChange = 10;
	else if (move.mSourceGroup == CardStackGroups::foundationGroup && move.mTargetGroup == CardStackGroups::pileGroup) move.mScoreChange = -15;
	else if (move.mSourceGroup == CardStackGroups::pileGroup && move.mTargetGroup == CardStackGroups::foundationGroup) move.mScoreChange = 10;
	else move.mScoreChange = 0;
	if (move.mSourceGroup == CardStackGroups::pileGroup && move.mPriorVisibility == false && move.mPriorVisibilityAltered == true) move.mScoreChange += 5;
	if (this->mScore < -move.mScoreChange) move.mScoreChange = -this->mScore;
	this->mScore += move.mScoreChange;
	this->mLastMoves.push_back(move);

	return true;
}

/**
* \brief reverts last move
*
* reverts last move, including score
*/
bool SolitaireBoard::undo()
{
	if (this->mLastMoves.size() == 0) return false;
	SolitaireGameMove& move = this->mLastMoves.back();
	switch (move.mMoveType) {
	case SolitaireGameMove::move:
		this->mStacks[move.mTargetGroup][move.mTargetIndex].moveCardIgnoringRules(
			this->mStacks[move.mSourceGroup][move.mSourceIndex],
			move.mCardAmount
		);
		if (move.mPriorVisibilityAltered) this->mStacks[move.mSourceGroup][move.mSourceIndex].setCardVisibility(move.mPriorVisibility, move.mCardAmount);
		break;
	case SolitaireGameMove::draw:
		this->mStacks[drawGroup][0].moveCardIgnoringRules(this->mStacks[deckGroup][0]);
		break;
	case SolitaireGameMove::recycle:
		while (!this->mStacks[deckGroup][0].isEmpty()) {
			this->mStacks[deckGroup][0].moveCardIgnoringRules(this->mStacks[drawGroup][0]);
		}
		break;
	}
	this->mScore -= move.mScoreChange;
	this->mLastMoves.pop_back();
	return true;

}

/**
* \brief draws card from deck
*
* draws card from deck, this class is 1 card per draw version
*/
void SolitaireBoard::draw()
{
	if (this->mStacks[deckGroup][0].getSize() == 0) return;
	this->mStacks[deckGroup][0].moveCardIgnoringRules(this->mStacks[drawGroup][0]);
	this->mLastMoves.push_back(SolitaireGameMove(SolitaireGameMove::draw, SolitaireBoard::CardStackGroups(), 0, SolitaireBoard::CardStackGroups(), 0, 0, false, false));
}

/**
* \brief Prepares the game
*
* prepares the game, clearing board, filling deck with cards in random order,and transferring them to piles
*/
void SolitaireBoard::initialDeal()
{
	this->clearBoard();
	this->mStacks[deckGroup][0].fillWithShuffledDeck();
	for (int i = 0; i < 7; i++) {
		this->mStacks[deckGroup][0].moveCardIgnoringRulesAndVisibility(this->mStacks[pileGroup][i], i);
		this->mStacks[deckGroup][0].moveCardIgnoringRules(this->mStacks[pileGroup][i]);
	}
}

/**
* \brief Recycles the stack of drawn cards into deck
*
* Recycles the stack of drawn cards into deck, updates score
*/
void SolitaireBoard::recycleDrawn()
{
	while (!this->mStacks[drawGroup][0].isEmpty()) {
		this->mStacks[drawGroup][0].moveCardIgnoringRules(this->mStacks[deckGroup][0]);
	}
	SolitaireGameMove record = SolitaireGameMove(SolitaireGameMove::recycle, SolitaireBoard::CardStackGroups(), 0, SolitaireBoard::CardStackGroups(), 0, 0, false, false);
	record.mScoreChange = -150;
	if (this->mScore < -record.mScoreChange) record.mScoreChange = -this->mScore;
	this->mScore += record.mScoreChange;
	this->mLastMoves.push_back(record);

}

/**
* \brief Determines possible moves
* \param[in] limit maximum number of moves returned
* \return vector containing possible moves
*
* finds moves in order
* pile->foundation
* pile->pile
* deck->pile
* deck->foundation
* draw(needs recycle)->foundation
* draw(needs recycle)->pile
*/
std::vector<SolitaireGameMove> SolitaireBoard::getHints(unsigned int limit) const
{
	std::vector<SolitaireGameMove> ret;

	int targetIndex = 0;
	int sourceIndex = 0;
	if (this->mStacks[drawGroup][0].getSize() > 0) {
		for (const CardStack& trgt : this->mStacks[pileGroup]) {
			if (this->mStacks[drawGroup][0].isMovePossible(trgt)) {
				ret.push_back(SolitaireGameMove(SolitaireGameMove::move, drawGroup, 0, pileGroup, targetIndex, 1, false, false));
				if (ret.size() == limit) return ret;
			}
			targetIndex++;
		}
		targetIndex = 0;
		sourceIndex = 0;
	}

	for (const CardStack& src : this->mStacks[pileGroup]) {
		for (const CardStack& trgt : this->mStacks[foundationGroup]) {
			if (src.isMovePossible(trgt)) {
				ret.push_back(SolitaireGameMove(SolitaireGameMove::move, pileGroup, sourceIndex, foundationGroup, targetIndex, 1, false, false));
				if (ret.size() == limit) return ret;
			}
			targetIndex++;
		}
		targetIndex = 0;
		sourceIndex++;
	}
	/*
	sourceIndex = 0;
	for (const CardStack& src : this->mStacks[deckGroup]) {
		int targetIndex = 0;
		for (const CardStack& trgt : this->mStacks[pileGroup]) {
			for (const Card& crd : trgt) {
				if (src.isMovePossible(trgt)) {
					ret.push_back(SolitaireGameMove(SolitaireGameMove::draw, deckGroup, 0, deckGroup, 0, 0, false, false));
					if (ret.size == limit) return ret;
				}
				targetIndex++;
			}
		}
		targetIndex = 0;
		sourceIndex++;
	}
	sourceIndex = 0;
	for (const CardStack& src : this->mStacks[deckGroup]) {
		int targetIndex = 0;
		for (const CardStack& trgt : this->mStacks[foundationGroup]) {
			for (const Card& crd : trgt) {
				if (src.isMovePossible(trgt)) {
					ret.push_back(SolitaireGameMove(SolitaireGameMove::draw, deckGroup, 0, deckGroup, 0, 0, false, false));
					if (ret.size == limit) return ret;
				}
				targetIndex++;
			}
		}
		targetIndex = 0;
		sourceIndex++;
	}
	*/
	sourceIndex = 0;
	targetIndex = 0;
	for (const CardStack& src : this->mStacks[pileGroup]) {
		for (const CardStack& trgt : this->mStacks[pileGroup]) {
				for (int amount = 1; amount <= src.getSize();  amount++) {
					if (src.isMovePossible(trgt,amount)) {
						ret.push_back(SolitaireGameMove(SolitaireGameMove::move, pileGroup, sourceIndex, pileGroup, targetIndex, amount, false, false));
						if (ret.size() == limit) return ret;
					}
				}
			targetIndex++;
		}
		targetIndex = 0;
		sourceIndex++;
	}

	if (this->mStacks[drawGroup][0].getSize() > 0){
		targetIndex = 0;
		for (const CardStack& trgt : this->mStacks[foundationGroup]) {
			if (this->mStacks[drawGroup][0].isMovePossible(trgt)) {
				ret.push_back(SolitaireGameMove(SolitaireGameMove::move, drawGroup, 0, foundationGroup, targetIndex, 1, false, false));
				if (ret.size() == limit) return ret;
			}
			targetIndex++;
		}
	}
	if (this->mStacks[deckGroup][0].getSize() > 0) {
		ret.push_back(SolitaireGameMove(SolitaireGameMove::draw, deckGroup, 0, deckGroup, 0, 0, false, false));
	}
	else {
		ret.push_back(SolitaireGameMove(SolitaireGameMove::recycle, deckGroup, 0, deckGroup, 0, 0, false, false));
	}
	return ret;
}

/**
* \brief get constant reference to chosen Cardstack
* \param[in] group stackgroup of stack
* \param[in] index index of stack within the group
* \return mValue of card
*/
const CardStack& SolitaireBoard::getStackRef(CardStackGroups group, int index) const
{
	const CardStack& ref = this->mStacks[group][index];
	return ref;
}

/**
* \brief getter for score
* \return curreent score
*/
int SolitaireBoard::getScore() const
{
	return this->mScore;
}
/**
* \brief a constructor
*/
SolitaireGameMove::SolitaireGameMove(MoveTypes moveType, SolitaireBoard::CardStackGroups sourceGroup, int sourceIndex, SolitaireBoard::CardStackGroups targetGroup, int targetIndex, int cardAmount, bool priorVisibility, bool priorVisibilityAltered) :
	mMoveType(moveType), mSourceGroup(sourceGroup), mSourceIndex(sourceIndex), mTargetGroup(targetGroup), mTargetIndex(targetIndex), mCardAmount(cardAmount), mPriorVisibility(priorVisibility), mPriorVisibilityAltered(priorVisibilityAltered) {}
SolitaireGameMove::SolitaireGameMove() {}

/**
* \brief Saves a board's status into a file
* \param[in] filename file to save into/create
*/
void SolitaireBoard::save(char* filename)
{
	std::ofstream savefile(filename);
	for (unsigned int i = 0; i < this->mStacks.size(); i++) {
		for (unsigned int j = 0; j < this->mStacks[i].size(); j++) {
			for (auto iter = this->mStacks[i][j].rbegin(); iter != this->mStacks[i][j].rend(); ++iter) {
				char visible;
				auto suit = this->suit2char.at((*iter).getSuit());
				auto value = this->value2str.at((*iter).getValue());
				if ((*iter).getVisible()) visible = 'V';
				else visible = 'I';
				savefile << " " << visible << suit << value;
			}
			savefile << std::endl;
		}
	}
	savefile << this->mScore;
}

/**
* \brief loads status of the board from a file
* \param[in] filename file to load from
* \return True if loading was successful, false otherwise
*/
bool SolitaireBoard::load(char* filename)
{
	this->clearBoard();
	std::ifstream savefile(filename);
	for (unsigned int i = 0; i < this->mStacks.size(); i++) {
		for (unsigned int j = 0; j < this->mStacks[i].size(); j++) {
			char separator;
			savefile.get(separator);
			while (separator == ' ') {
				char visibilitychar, suitchar;
				std::string valuestring;

				savefile >> visibilitychar >> suitchar >> valuestring;
				savefile.get(separator);

				bool visibility;
				if (visibilitychar == 'V') visibility = true;
				else visibility = false;
				Card::CardSuits suit = char2suit.at(suitchar);
				Card::CardValues value = str2value.at(valuestring);

				mStacks[i][j].addCard(Card(visibility, value, suit));
			}
			if (savefile.eof()) return false;
		}
	}
	savefile >> this->mScore;
	return true;
}

/**
* \brief Clears the status of the board, erasing score and move history and emptying all stacks
*/
void SolitaireBoard::clearBoard()
{
	this->mScore = 0;
	this->mLastMoves.clear();
		for (unsigned int i = 0; i < this->mStacks.size(); i++)
			for (unsigned int j = 0; j < this->mStacks[i].size(); j++)
				this->mStacks[i][j].clearStack();
}

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
	const Card& priorCard = *this->mStacks[sourceGroup][sourceIndex].begin();
	SolitaireGameMove newmove = SolitaireGameMove(SolitaireGameMove::move,sourceGroup, sourceIndex, targetGroup, targetIndex, amount, priorCard.getVisible(), false);
	if (!this->mStacks[sourceGroup][sourceIndex].moveCard(mStacks[targetGroup][targetIndex], amount)) return false;
	newmove.mPriorVisibilityAltered = newmove.mPriorVisibility != priorCard.getVisible();
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
	auto iter = this->mStacks[move.mSourceGroup][move.mSourceIndex].begin();
	if (this->mStacks[move.mSourceGroup][move.mSourceIndex].getSize() > 1) iter++;
	const Card& priorCard = *iter;
	move.mMoveType = SolitaireGameMove::move;
	move.mPriorVisibility = priorCard.getVisible();
	if (!this->mStacks[move.mSourceGroup][move.mSourceIndex].moveCard(mStacks[move.mTargetGroup][move.mTargetIndex], move.mCardAmount)) return false;
	if (this->mStacks[move.mSourceGroup][move.mSourceIndex].getSize() > 1) move.mPriorVisibilityAltered = false;
	else move.mPriorVisibilityAltered = move.mPriorVisibility != priorCard.getVisible();
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
* prepares the game, filling deck with random cards, and transferring them to piles
*/
void SolitaireBoard::initialDeal()
{
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
	this->mLastMoves.push_back(SolitaireGameMove(SolitaireGameMove::recycle, SolitaireBoard::CardStackGroups(), 0, SolitaireBoard::CardStackGroups(), 0, 0, false, false));
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
	int sourceIndex = 0;
	for (const CardStack& src : this->mStacks[pileGroup]) {
		int targetIndex = 0;
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
	for (const CardStack& src : this->mStacks[pileGroup]) {
		int targetIndex = 0;
		for (const CardStack& trgt : this->mStacks[pileGroup]) {
				for (int amount = 1; amount < src.getSize();  amount++) {
					if (src.isMovePossible(trgt),amount) {
						ret.push_back(SolitaireGameMove(SolitaireGameMove::move, pileGroup, sourceIndex, pileGroup, targetIndex, amount, false, false));
						if (ret.size() == limit) return ret;
					}
				targetIndex++;
			}
		}
		targetIndex = 0;
		sourceIndex++;
	}
	sourceIndex = 0;
	if (this->mStacks[deckGroup][0].getSize() > 0) {
		ret.push_back(SolitaireGameMove(SolitaireGameMove::draw, deckGroup, 0, deckGroup, 0, 0, false, false));
		if (ret.size() == limit) return ret;
	}
	else {
		for (const CardStack& src : this->mStacks[drawGroup]) {
			int targetIndex = 0;
			for (const CardStack& trgt : this->mStacks[pileGroup]) {
					for (int amount = 1; amount < src.getSize(); amount++) {
						if (src.isMovePossible(trgt), amount) {
							ret.push_back(SolitaireGameMove(SolitaireGameMove::recycle, pileGroup, sourceIndex, pileGroup, targetIndex, amount, false, false));
							if (ret.size() == limit) return ret;
						}
					}
					targetIndex++;
			}
			targetIndex = 0;
			sourceIndex++;
		}
	}
	return ret;
}

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

SolitaireGameMove::SolitaireGameMove(MoveTypes moveType, SolitaireBoard::CardStackGroups sourceGroup, int sourceIndex, SolitaireBoard::CardStackGroups targetGroup, int targetIndex, int cardAmount, bool priorVisibility, bool priorVisibilityAltered) :
	mMoveType(moveType), mSourceGroup(sourceGroup), mSourceIndex(sourceIndex), mTargetGroup(targetGroup), mTargetIndex(targetIndex), mCardAmount(cardAmount), mPriorVisibility(priorVisibility), mPriorVisibilityAltered(priorVisibilityAltered) {}
SolitaireGameMove::SolitaireGameMove() {}

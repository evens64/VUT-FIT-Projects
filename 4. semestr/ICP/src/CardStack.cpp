// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)
#include "CardStack.h"


/**
* \brief A default constructor
*/
CardStack::CardStack()
{
}

/**
* \brief A constructor
* \param visibility Visibility rule of new object
* \param enabledSuits Suit rule of new object
* \param order Order rule of new object
* \param cardRemovable removability of cards rule of new object
* \param multiMove Specifies if multiple cards can be moved to and from this stack at once
*/
CardStack::CardStack
(StackVisibilityRules visibility, CardSuitRules enabledSuits, OrderRules order, bool cardsRemovable, bool multiMove) :
mVisibility(visibility), mEnabledSuit (enabledSuits), mOrder (order), mCardsRemovable(cardsRemovable), mMultiMove(multiMove)
{
}

/**
* \brief A Trivial Destructor
*/
CardStack::~CardStack()
{
}

void CardStack::clearStack() {
	this->mStack.clear();
}

/**
* \brief Fills stack with shuffled Deck
*
* Fills stack with shuffled Deck, all cards of the deck are inserted, this is used on the deck stack when starting a game
*/
void CardStack::fillWithShuffledDeck()
{
	for (int i = 0; i < 4; i++) {				// CardSuits
		for (int j = 1; j <= Card::CardValues::last; j++) {			// CardValue
			this->addCard(Card(static_cast <Card::CardValues>(j), static_cast<Card::CardSuits>(i)));
		}
	}
	srand((unsigned int)time(NULL));
	std::random_shuffle(this->mStack.begin(), this->mStack.end());
}

/**
* \brief Adds a card on top of the stack
* \param[in] crd Card to be added
*
* Adds a card on top of the stack, does not check whether rules allow it, Visibility of the card is not checked or modified
*/
void CardStack::addCard(const Card& crd)
{
	this->mStack.push_front(crd);
}
/**
* \brief Adds a card on top of the stack, rvalue reference version
* \param[in] crd Card to be added
*
* Adds a card on top of the stack, does not check whether rules allow it, Visibility of the card is not checked or modified
* Uses std::forward
*/
void CardStack::addCard(Card&& crd)
{
	this->mStack.push_front(std::forward<Card>(crd));
}

/**
* \brief Moves top card of this stack to another
* \param[in] stck Stack of cards to move card to
* \return True if Card was moved, False if it was not moved due to violation of rules
*
* Moves card from this stack to another,
* checks whether rules allow the move, can use functions addCard(), isMovePossible() and by extenstion isCardAcceptable(),
* modifies the visibilities of both the moved card and top card of the other deck according to visibility rules of decks
*/
bool CardStack::moveCard(CardStack& stck)
{
	if (this->isMovePossible(stck) == true) {
		if (stck.getSize() > 0 && (stck.getVisibility() == stackNotVisible || stck.getVisibility() == stackOnlyTopVisible)) stck.mStack.front().setVisible(false);
		stck.addCard(this->mStack.front());
		stck.mStack.front().setVisible(!(stck.getVisibility() == stackNotVisible));
		this->mStack.pop_front();
		if (this->getSize() > 0) this->mStack.front().setVisible(!(this->getVisibility() == stackNotVisible));
		return true;
	}
	return false;
}

/**
* \brief Moves a substack of cards from this stack to another
* \param[in] stck Stack of cards to move cards to
* \param[in] amount number of cards to move
* \return True if Card was moved, False if it was not moved due to violation of rules
*
* Moves a substack of cards from this stack to another,
* checks whether rules allow the move, can use functions addCard() and isMovePossible()
* modifies the visibilities of moved cards, and new top card of this stack according to visibility rules of stacks
*/
bool CardStack::moveCard(CardStack& stck, int amount) {
	if (this->isMovePossible(stck, amount) == true) {
		int temp = amount;
		while (temp != 0) {
			if (stck.getSize() > 0 && (stck.getVisibility() == stackNotVisible || stck.getVisibility() == stackOnlyTopVisible)) stck.mStack.front().setVisible(false);
			stck.addCard(this->mStack[temp - 1]);
			temp--;
		}
		stck.mStack.front().setVisible(!(stck.getVisibility() == stackNotVisible));
		temp = amount;
		while (temp != 0) {
			this->mStack.pop_front();
			temp--;
		}
		if (this->getSize() > 0) this->mStack.front().setVisible(!(this->getVisibility() == stackNotVisible));
		return true;
	}
	return false;
}

/**
* \brief Moves card from this stack to another, disregarding rules for valid movement
* \param[in] stck Stack of cards to move card to
*
* Moves card from this stack to another, ignores rules for moving cards, but sets cisibility
*/
void CardStack::moveCardIgnoringRules(CardStack& stck)
{
	if (stck.getSize() > 0 && (stck.getVisibility() == stackNotVisible || stck.getVisibility() == stackOnlyTopVisible)) stck.mStack.front().setVisible(false);
	stck.addCard(this->mStack.front());
	stck.mStack.front().setVisible(!(stck.getVisibility() == stackNotVisible));
	this->mStack.pop_front();
	if (this->getSize() > 0) this->mStack.front().setVisible(!(this->getVisibility() == stackNotVisible));
}

/**
* \brief Moves a substack of cards from this stack to another, disregarding rules for valid movement
* \param[in] stck Stack of cards to move cards to
* \param[in] amount Amount of cards to move
*
* Moves cards from this stack to another, ignores rules for moving cards
*/
void CardStack::moveCardIgnoringRules(CardStack& stck, int amount)
{
	int temp = amount;
	while (temp != 0) {
		if (stck.getSize() > 0 && (stck.getVisibility() == stackNotVisible || stck.getVisibility() == stackOnlyTopVisible)) stck.mStack.front().setVisible(false);
		stck.addCard(this->mStack[temp - 1]);
		temp--;
	}
	stck.mStack.front().setVisible(!(stck.getVisibility() == stackNotVisible));
	temp = amount;
	while (temp != 0) {
		this->mStack.pop_front();
		temp--;
	}
	if (this->getSize() > 0) this->mStack.front().setVisible(!(this->getVisibility() == stackNotVisible));
}


/**
* \brief Moves card from this stack to another, disregarding rules for valid movement and visibility
* \param[in] stck Stack of cards to move card to
*
* Moves card from this stack to another, ignores rules for moving cards
*/
void CardStack::moveCardIgnoringRulesAndVisibility(CardStack& stck)
{
	stck.addCard(this->mStack.front());
	this->mStack.pop_front();
}

/**
* \brief Moves a substack of cards from this stack to another, disregarding rules for valid movement
* \param[in] stck Stack of cards to move cards to
* \param[in] amount Amount of cards to move
*
* Moves cards from this stack to another, ignores rules for moving cards
*/
void CardStack::moveCardIgnoringRulesAndVisibility(CardStack& stck, int amount)
{
	int temp = amount;
	while (temp != 0) {
		stck.addCard(this->mStack[temp - 1]);
		temp--;
	}
	temp = amount;
	while (temp != 0) {
		this->mStack.pop_front();
		temp--;
	}
}

/**
* \brief Checks whether moving top card from this deck to the other is allowed
* \param[in] stck Stack of cards to check against
* \return True if move was valid, False otherwise
*
* Checks whether moving top card from this deck to the other is allowed, this depends on rules of both decks,
* can use isCardAcceptable()
*/
bool CardStack::isMovePossible(const CardStack& stck) const
{
	if (this->mCardsRemovable && stck.isCardAcceptable(this->mStack.front())) return true;
	else return false;
}

/**
* \brief Checks whether moving substack of cards from this deck to the other is allowed
* \param[in] stck Stack of cards to check against
* \param[in] amount of cards involeved in move
* \return True if move is valid, False otherwise
*
* Checks whether moving substack of cards from this deck to the other is allowed, this depends on rules of both decks,
*/
bool CardStack::isMovePossible(const CardStack& stck, unsigned int amount) const
{
	if (!this->mCardsRemovable) return false;	// nelze odstranit
	if (amount > 1 && !(this->mMultiMove && stck.mMultiMove)) return false;
	if (amount > this->mStack.size()) return false;					// failsafe
	if (!stck.isCardAcceptable(this->mStack[amount-1])) return false;
	for (int i = amount - 2; i >= 0; i--) {
		if (!stck.isCardAcceptable(this->mStack[i], this->mStack[i + 1])) return false;
	}
	return true;
}

/**
* \brief Checks whether moving card to this deck is allowed
* \param[in] crd Card to check against
* \return True if card can be accepted, False otherwise
*
* Checks whether moving certain card to this deck is allowed,
*/
bool CardStack::isCardAcceptable(const Card& crd) const {
	if (this->mStack.size() > 0) return isCardAcceptable(crd, this->mStack.front());
	else return isCardAcceptable(crd, Card(Card::blindcard, Card::blindsuit));
}
/**
* \brief Checks whether moving card to this deck is allowed, if given card would be on top
* \param[in] crd Card to check against
* \param[in] topcard Presumed card on top
* \return True if card can be accepted, False otherwise
*
* Checks whether moving certain card to this deck is allowed,
*/
bool CardStack::isCardAcceptable(const Card& crd, const Card& topcard) const {
	switch (this->mEnabledSuit) {
		case CardSuitRules::alternating:
			if (crd.getColor() == topcard.getColor())
				return false;
			break;
		case CardSuitRules::hearts:
		case CardSuitRules::diamonds:
		case CardSuitRules::clubs:
		case CardSuitRules::spades:
			if (crd.getSuit() != this->mEnabledSuit)
				return false;
			break;
		case CardSuitRules::black:
			if (crd.getSuit() != Card::spades && crd.getSuit() != Card::clubs)
				return false;
			break;
		case CardSuitRules::red:
			if (crd.getSuit() != Card::hearts && crd.getSuit() != Card::diamonds)
				return false;
			break;
		default:
			break;
	}
	switch (this->mOrder) {
		case OrderRules::descendingOrder:
		if (!crd.Follows(topcard))
			return false;
		break;
		case OrderRules::ascendingOrder:
			if (!crd.Precedes(topcard))
				return false;
			break;
		case OrderRules::noOrder:
			return false;
		default:
			break;
	}
	if (crd.getVisible() == false) return false;
	return true;
}


/**
* \brief Checks whether card this stack is complete, that means top card is Ace
* \return True stack is complete, False otherwise
*
* Checks whether card stack is complete, eg. top card is Ace,
* other cards are disregarded, since only sensible use is on foundation stacks
*/
bool CardStack::isComplete() const
{
	bool complete = true;
	int value = 1;
	for (auto c: this->mStack) {
		if (c.getValue() != value) {
			complete = false;
			break;
		}
		value++;
	}

	return complete;
}


/**
* \brief Getter for constant iterator of cardstack
* \return Iterator to beggining of stscks deque
*/
std::deque<Card>::const_iterator CardStack::begin() const
{
	return this->mStack.begin();
}

/**
* \brief Getter for constant iterator of cardstack
* \return Iterator to the end of stack's deque
*/
std::deque<Card>::const_iterator CardStack::end() const
{
	return this->mStack.end();
}

/**
* \brief Getter for reverse of cardstack
* \return reverse Iterator to reverse beggining of stscks deque
*/
std::deque<Card>::const_reverse_iterator CardStack::rbegin() const
{
	return this->mStack.rbegin();
}

/**
* \brief Getter for reverse iterator of cardstack
* \return reverse Iterator to the reverse end of stack's deque
*/
std::deque<Card>::const_reverse_iterator CardStack::rend() const
{
	return this->mStack.rend();
}

Card CardStack::front() const{
	return this->mStack.front();
}
/**
* \brief Setter for visibility rule of cardstack
* \param[in] rule Visibility rule to set
*/
void CardStack::setVisibility(StackVisibilityRules rule)
{
	this->mVisibility = rule;
}

/**
* \brief Setter for OrderRule of cardstack
* \param[in] rule Rule to set
*/
void CardStack::setOrder(OrderRules rule)
{
	this->mOrder = rule;
}

/**
* \brief Setter for enabled suit of cardstack
* \param[in] rule for accepting suits
*/
void CardStack::setSuitRule(CardSuitRules rule)
{
	this->mEnabledSuit = rule;
}

/**
* \brief sets whether the cards are removable
* \param[in] rule Rule for removability of cards
*/
void CardStack::setCardsRemovable(bool removable) {
	this->mCardsRemovable = removable;
}

/**
* \brief sets whether the cards are removable
* \param[in] rule Rule for removability of cards
*/
void CardStack::setMultiMove(bool multiMove) {
	this->mMultiMove = multiMove;
}


/**
* \brief getter for visibility rule of cardstack
* \return cardstack's visibility
*/
CardStack::StackVisibilityRules CardStack::getVisibility() const
{
	return this->mVisibility;
}

/**
* \brief getter for order rule of cardstack
* \return Order in which cardstack accepts cards
*/
CardStack::OrderRules CardStack::getOrder() const
{
	return this->mOrder;
}

/**
* \brief getter for accepted suit of cardstack
* \return suit which cardstack accepts
*/
CardStack::CardSuitRules CardStack::getSuitRule() const
{
	return this->mEnabledSuit;
}

/**
* \brief getter for size of stack
* \return number of cards in stack
*/
int CardStack::getSize() const
{
	return (int)this->mStack.size();
}
/**
* \brief Sets visibility of chosen card
* \param[in] visibility Value to set
* \param[in] index Index of card to modify
*/
void CardStack::setCardVisibility(bool visibility, int index) {
	this->mStack[index].setVisible(visibility);
}

/**
* \brief returns true if stack is empty
* \return emptiness of stack
*/
bool CardStack::isEmpty() {
	return this->mStack.size() == 0;
}

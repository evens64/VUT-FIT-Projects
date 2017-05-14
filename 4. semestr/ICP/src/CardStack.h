// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef CARDSTACK_H_INCLUDED
#define CARDSTACK_H_INCLUDED

#include <algorithm>	// std::random_shuffle
#include <deque>
#include "Card.h"
#include <time.h>


/**
* \brief Class describing a stack of cards in the game, such as the deck, foundations and the tableau piles
*/
class CardStack
{
public:
	/**
	* \brief Enumerator for rules about order of cards in stacks
	*/
	enum OrderRules {
		anyOrder,  /**< stack accepts cards in any order (turned cards stacks)*/
		descendingOrder, /**< stack accepts cards only in descending order (piles)*/
		ascendingOrder, /**< stack accepts card only in ascending order (foundations)*/
		noOrder /**<stack does not accept cards*/
	};

	/**
	* \brief Enumerator for rules fo visibility of cards in stack
	*/
	enum StackVisibilityRules {
		stackNotVisible, /**< None of the cards in the stack are ever visible*/
		stackTopMadeVisible, /**< Top card of the stack is always made visible, and left visible if more cards are added*/
		stackOnlyTopVisible /**< Only top card of the stack is ever visible*/
	};

	enum CardSuitRules {
		hearts = Card::hearts,
		diamonds = Card::diamonds,
		clubs = Card::clubs,
		spades = Card::spades,
		red,
		black,
		anySuit,
		alternating
	};

protected:

	StackVisibilityRules mVisibility; /**< Rule about visibility of cards in the card stack*/

	CardSuitRules mEnabledSuit; /**< Suit that can be put into card stack, 0 or anySuit for all */
	OrderRules mOrder; /**< order of cards the card accepts cards in (any, descending, ascending)*/
	bool mCardsRemovable; /**< Boolean determining whether cards from the stack can be moved or not*/
	bool mMultiMove; /**< Boolean determining whether multiple cards can be moved at once*/
public:
	CardStack();
	CardStack(StackVisibilityRules visibility, CardSuitRules enabledSuits, OrderRules morder, bool cardsRemovable, bool multiMove);
	~CardStack();

	std::deque<Card> mStack; /**< Deque containing cards of the card stack, top card is the first item in queue*/

	void fillWithShuffledDeck();
	void addCard(const Card& crd);
	void addCard(Card&& crd);
	bool moveCard(CardStack& stck);
	bool moveCard(CardStack& stck, int amount);
	void moveCardIgnoringRules(CardStack& stck);
	void moveCardIgnoringRules(CardStack& stck, int amount);
	void moveCardIgnoringRulesAndVisibility(CardStack & stck);
	void moveCardIgnoringRulesAndVisibility(CardStack & stck, int amount);
	bool isMovePossible(const CardStack& stck) const;
	bool isMovePossible(const CardStack& stck, unsigned int amount) const;
	bool isCardAcceptable(const Card& crd) const;
	bool isCardAcceptable(const Card& crd, const Card& topcard) const;
	bool isComplete() const;

	std::deque<Card>::const_iterator begin() const;
	std::deque<Card>::const_reverse_iterator rbegin() const;
	std::deque<Card>::const_iterator end() const;

	std::deque<Card>::const_reverse_iterator rend() const;

	void setVisibility(StackVisibilityRules rule);
	void setOrder(OrderRules rule);
	void setSuitRule(CardSuitRules suit);
	void setCardsRemovable(bool removable);
	void setMultiMove(bool multiMove);

	StackVisibilityRules getVisibility() const;
	OrderRules getOrder() const;
	CardSuitRules getSuitRule() const;
	int getSize() const;
	void setCardVisibility(bool visibility, int index);
	bool isEmpty();
};

#endif

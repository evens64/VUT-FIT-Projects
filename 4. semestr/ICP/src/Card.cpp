// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)
#include "Card.h"

/** \brief A constructor
* \param[in] value Value of the new card (1-10, john, queen, king)
* \param[in] suit Suit of the new card(hearts, diamonds, clubs, spades)
* A constructor, all cards start invisible
*/
Card::Card(CardValues value, CardSuits suit)
{
	this->mValue = value;
	this->mSuit = suit;
	this->mVisible = false;
}

/**
* \brief A constructor
* \param[in] visible initial visibility of card
* \param[in] value Value of the new card (1-10, john, queen, king)
* \param[in] suit Suit of the new card(hearts, diamonds, clubs, spades)
* \returns mValue of card
*/
Card::Card(bool visible, CardValues value, CardSuits suit)
{
	this->mValue = value;
	this->mSuit = suit;
	this->mVisible = visible;
}

/**
* \brief A trivial destructor
*/
Card::~Card()
{
}



/**
* \brief Determines if this card's value follows value of other card
* \param[in] crd Card for this card to be compared to
* \return True if value of this card is lesser than value of crd by one, False otherwise
*/
bool Card::Follows(const Card& crd) const
{
	return (this->mValue == crd.mValue - 1 || ((this->mValue == king) && (crd.mValue == blindcard)));
}

/**
* \brief Determines if this card's value precedes value of other card
* \param[in] crd Card for this card to be compared to
* \return True if value of this card is one greater than value of crd, False otherwise
*/
bool Card::Precedes(const Card& crd) const
{
	return (this->mValue == crd.mValue + 1 || ((this->mValue == ace) && (crd.mValue == blindcard)));
}
/**
* \brief Determines if this card's suit is the same as other card's
* \param[in] crd Card for this card to be compared to
* \return True if the suits are same, False otherwise
*/
bool Card::isSameSuit(const Card& crd) const
{
	return this->mSuit == crd.mSuit ? true : false;
}
/**
* \brief Determines if this card's suit is the suit in parameter
* \param[in] suit Suit for this card to be compared to
* \return True if the card has this suit, False otherwise
*/
bool Card::isSuit(CardSuits suit) const
{
	return this->mSuit == suit ? true : false;
}

/**
* \brief sets visibility of card
* \param[in] visible False for invisible, True for visible
*/
void Card::setVisible(bool visible)
{
	this->mVisible = visible;
}

/**
* \brief gets visibility of card
* \return visible False for invisible, True for visible
*/

bool Card::getVisible() const
{
	return this->mVisible;
}

/**
* \brief gets value of card
* \return mValue of card
*/
Card::CardValues Card::getValue() const
{
	return this->mValue;
}

/**
* \brief gets suit of card
* \return mSuit of card
*/
Card::CardSuits Card::getSuit() const
{
	return this->mSuit;
}

/**
* \brief gets value of card
* \return mValue of card
*/
Card::CardColors Card::getColor() const {
	if (this->mSuit == diamonds || this->mSuit == hearts) return red;
	if (this->mSuit == blindsuit) return blind;
	else return black;
}


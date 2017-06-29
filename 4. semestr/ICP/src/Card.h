// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

/**
* \brief Class describing a card in the game
*/
class Card
{
public:
	/// card values
	enum CardValues { ace = 1, two = 2, three = 3, four = 4, five = 5, six = 6, seven = 7, eight = 8, nine = 9, ten = 10, jack = 11, queen = 12, king = 13, blindcard = 14, first = ace, last = king };
	/// card suits
	enum CardSuits { hearts, diamonds, clubs, spades, blindsuit };
	/// card colors
	enum CardColors { red, black, blind };
protected:
	CardValues mValue; /**< value of card, 1,2,3 John, king etc.*/
	CardSuits mSuit; /**< suit of card, hearts, diamonds, clubs, spades.*/
	bool mVisible; /**< visibility of the card, True=visible, False=invisible */

public:
	Card(CardValues value, CardSuits suit);
	Card(bool visible, CardValues value, CardSuits suit);
	~Card();

	bool Follows(const Card& crd) const;
	bool Precedes(const Card& crd) const;
	bool isSameSuit(const Card& crd) const;
	bool isSuit(CardSuits suit) const;
	void setVisible(bool visible);
	bool getVisible() const;
	CardValues getValue() const;
	CardSuits getSuit() const;
	CardColors getColor() const;
	//you might want to add isRed/isBlack to symplify things.
};

#endif

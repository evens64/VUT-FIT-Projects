#include "GUISolitaireBoard.h"
/**
* \brief Gets Cards the GUI should display in given stack
* param[in] stackgroup group of stack
* param[in] stack index of stack within group
* param[out] arr array to fill with CardIDs
*/
void GUISolitaireBoard::GetStackIDs(unsigned int stackgroup, unsigned int stack, CardID arr [19]) {
	unsigned int i = 0;
	if (this->mStacks[stackgroup][stack].getSize() < 1){
		arr[0] = CardID(4,4); //prázdný stack
		i++;
	}
	for (auto iter = this->mStacks[stackgroup][stack].rbegin();iter != this->mStacks[stackgroup][stack].rend(); iter++, i++){
		if (!(*iter).getVisible()) arr[i] = CardID(4,5);//převrácená karta
		else arr[i] = CardID((*iter).getSuit(),(*iter).getValue()-1);
	}
	for (;i<19;i++)
		arr[i] = CardID(4,6); //žádná karta
}

/**
* \brief Gets card the GUI should display in given stack represented in GUI only by top card
* param[in] stackgroup group of stack
* param[in] stack index of stack within group
* return card to display
*/
CardID GUISolitaireBoard::GetTopCardID(unsigned int stackgroup, unsigned int stack) {
	if (this->mStacks[stackgroup][stack].getSize() < 1) {
		if (stackgroup == SolitaireBoard::foundationGroup)
			return CardID(4,stack);//karta pro foundation
		return CardID(4,4);//prázdný stack
	}
	if (!this->mStacks[stackgroup][stack].front().getVisible()) return CardID(4,5); //převrácená karta
	return CardID(this->mStacks[stackgroup][stack].front().getSuit(), this->mStacks[stackgroup][stack].front().getValue()-1);
}

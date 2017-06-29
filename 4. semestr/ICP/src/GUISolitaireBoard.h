#ifndef GUISOLITAIREBOARD_H
#define GUISOLITAIREBOARD_H
#include "SolitaireBoard.h"
#include "Structs.h"

/**
* \brief Class representing a playing board, enriched for methods related to communication between QUI and game logic
*/
class GUISolitaireBoard : public SolitaireBoard
{
public:
	CardID GetTopCardID(unsigned int stackgroup, unsigned int stack);
	void GetStackIDs(unsigned int stackgroup, unsigned int stack, CardID array [19]);
};

#endif // GUISOLITAIREBOARD_H

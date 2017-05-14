// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)
#include "ConsoleSolitaireGame.h"

ConsoleSolitaireGame::ConsoleSolitaireGame()
{
}


ConsoleSolitaireGame::~ConsoleSolitaireGame()
{
}
/**
* \brief Writes out status of all boards into standard output
*
* Writes out status of all boardd into standard output
*
* Format:
* two lines of #######
* board sections seperated by lines of ####
*/
void ConsoleSolitaireGame::writeoutAllBoards() const
{
	for (int i = 0; i < 4; i++) {
		if (mBoardsActive[i] == true) {
			std::cout << "###################################################" << std::endl;
			std::cout << "BOARD " << i + 1 << std::endl;
			std::cout << "###################################################" << std::endl;
			this->mBoards[i].writeoutGameStatus();
		}
	}
}


void ConsoleSolitaireGame::mainLoop()
{
	std::cout << "###################################################" << std::endl;
	std::cout << "SOLITAIRE the hard way"<< std::endl;
	std::cout << "###################################################" << std::endl;
	std::cout << "Enter command 'H' for help" << std::endl;
	while (1)
	if (this->executeCommand())
		this->writeoutAllBoards();
}
/**
* \brief Waits for and performs move of the player
* \return True if status of the game has changed, false otherwise
*
* Waits for and performs move of the player, maintains mGamesActive
* Input: ID of game or H for help, (IDs start at 1, for plebians)
* letter, D for draw, R for recyle, H for hint, L space [filename] for load, S space [filename] for save, N for newgame, or letter of stackgroup for move
* ID of stack in stackgroup, if input is move
* letter of stackgroup to move to, if input is move
* Id of stack in target group, if input is move
* space
* number of cards to move, hints to recieve
*
* examples: 2P1P2 2, 1D, 1H 3
*
* If move is invalid, displays message, newline, and awaits another input
* If hint is requested, displays hints in same format as input, newline, and awaits another input
*
* Writes out BIIIIG ezestriking message if game was won
*/
bool ConsoleSolitaireGame::executeCommand()
{
	char c1, c2;
	int util;
	std::string str;
	SolitaireGameMove move;
	std::cin >> c1;
	switch (c1) {
		default:
			this->writeInvalidCommand();
			return false;
		case 'H':
			this->printHelp();
			return false;
		case '1':
		case '2':
		case '3':
		case '4':
			std::cin >> c2;
			switch (c2) {
			case 'D':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				this->mBoards[c1 - '1'].draw();
				return true;
			case 'R':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				this->mBoards[c1 - '1'].recycleDrawn();
				return true;
			case 'U':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				if (this->mBoards[c1 - '1'].undo() == false){
					std::cout << "no moves to undo" << std::endl;
					return false;
				}
				return true;
			case 'H':
				//this->mBoards[c1 - '1'].writeoutHints();
				return false;
			case 'N':
				(&(this->mBoards[c1 - '1']))->~ConsoleSolitaireBoard();
				new (&(this->mBoards[c1 - '1'])) ConsoleSolitaireBoard();
				this->mBoards[c1 - '1'].initialDeal();
				this->mBoardsActive[c1 - '1'] = true;
				//this->mBoards[c1 - '1'] = ConsoleSolitaireBoard();
				return true;
			case 'L':
				std::cin >> c2;
				std::cin >> str;
				//this->mBoards[c1 - '1'].load(str);
				return true;
			case 'S':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				std::cin >> c2;
				std::cin >> str;
				//this->mBoards[c1 - '1'].save(str);
				return true;
			case 'P':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				move.mSourceGroup = SolitaireBoard::pileGroup;
				break;
			case 'W':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				move.mSourceGroup = SolitaireBoard::drawGroup;
				break;
			case 'F':
				if (this->mBoardsActive[c1 - '1'] == false) {
					std::cout << "board is inactive, type [boardnumber]N to start a new game" << std::endl;
					return false;
				}
				move.mSourceGroup = SolitaireBoard::foundationGroup;
				break;
			default:
				this->writeInvalidCommand();
				return false;
			}

			util = -1;
			std::cin >> util;
			move.mSourceIndex = util;
			std::cin >> c2;
			switch (c2) {
			case 'P':
				move.mTargetGroup = SolitaireBoard::pileGroup;
				break;
			case 'W':
				move.mTargetGroup = SolitaireBoard::drawGroup;
				break;
			case 'F':
				move.mTargetGroup = SolitaireBoard::foundationGroup;
				break;
			default:
				this->writeInvalidCommand();
				return false;
			}

			util = -1;
			std::cin >> util;
			move.mTargetIndex = util;

			util = -1;
			std::cin >> util;
			move.mCardAmount = util;
			this->mBoards[c1 - '1'].move(move);

	}
	return true;
}

/**
* \brief Print help for game
*/
void ConsoleSolitaireGame::printHelp() {
	std::cout << "HELP:" << std::endl;
	std::cout << "'H' for help" << std::endl;
	std::cout << "[gameid][letter] for various actions" << std::endl;
	std::cout << "\t- gameid 1-4 for ID of game" << std::endl;
	std::cout << "\t'D' for card draw" << std::endl;
	std::cout << "\t'R' for recycle drawn" << std::endl;
	std::cout << "\t'U' for undo last move" << std::endl;
	std::cout << "\t'H [count]' for hints (not implemented)" << std::endl;
	std::cout << "\t'L [filename]' for load game (not implemented)" << std::endl;
	std::cout << "\t'S [filename]' for save game (not implemented)" << std::endl;
	std::cout << "\t'N' for new game" << std::endl << std::endl;
	std::cout << "'[gameid][letterfrom][idfrom][letterto][idto] [count]' for moving cards" << std::endl;
	std::cout << "\t- gameid 1-4 for ID of game" << std::endl;
	std::cout << "\t- letterftom == [P|W|F] take card from" << std::endl;
	std::cout << "\t- idfrom == ID of stackgroup from which take" << std::endl;
	std::cout << "\t- letterto == [P|W|F] drop card to" << std::endl;
	std::cout << "\t- idto == ID of stackgroup where to drop" << std::endl;
	std::cout << "\t- count == count of cards to move" << std::endl;
	std::cout << "EXAMPLES: 2P1P2 2, 1D, 1H 3" << std::endl;
}

/**
* \brief Print error in stderr
*/
void ConsoleSolitaireGame::writeInvalidCommand() {
	std::cout << "INVALID COMMAND!!!" << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

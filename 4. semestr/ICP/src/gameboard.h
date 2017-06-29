// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QDrag>
#include <QFrame>
#include <QMessageBox>
#include <QtWidgets>
#include <iostream>
#include <array>

#include "GUISolitaireBoard.h"
#include "GameBoardCardStack.h"
#include "GameBoardTopCard.h"
#include "Card.h"

/**
* \brief GUI element representing a playing board
*/
class GameBoard : public QFrame
{
	Q_OBJECT
public:
	explicit GameBoard(QWidget *parent, bool state, const std::array<std::array<QIcon *,13>,5> &icons);
	virtual ~GameBoard();
	void Build(const std::array<std::array<QIcon *,13>,5> &icons);
	void Activate(bool doit = false);
	void Deactivate(bool doit = false);
	bool isActive();

	void notify(QString text);
	void update();
	void unHighlightAll();

private:
	GUISolitaireBoard *logicboard = NULL;
	bool state = false;
	QLabel *scoreLabel;
	QLabel *notificationLabel;
	QTimer *notificationTimer;

	GameBoardCardStack *pilestacks[7];
	GameBoardTopCard *deckstack;
	GameBoardTopCard *wastestack;
	GameBoardTopCard *foundationstacks[4];

	bool cardselected = false;
	SolitaireGameMove move;

public slots:
	void clearNotification();
	void onCardClick(int intclck);
	void btnClick_newGame();
	void btnClick_saveGame();
	void btnClick_loadGame();
	void btnClick_undoMove();
	void btnClick_hintMove();
	void btnClick_quitGame();


signals:

public slots:
};

#endif // GAMEBOARD_H

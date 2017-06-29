#ifndef GAMEBOARDTOPCARD_H
#define GAMEBOARDTOPCARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <array>
#include "Structs.h"
#include "CardButton.h"

/**
* \brief GUI element representing a stack of cards from which only top card is visible
*/
class GameBoardTopCard : public QWidget
{
	Q_OBJECT
public:
	explicit GameBoardTopCard(QWidget *parent, unsigned int stackgroup, unsigned int stack, const std::array<std::array<QIcon *,13>,5> &icons);
	void update(CardID newcard);
	bool isEmpty() const;
	void highlight(QString color);
	void unHighlight();
private:
	bool empty;
	const std::array<std::array<QIcon *,13>,5> icons;
	CardButton * card;
	bool highlighted = false;
signals:

public slots:
};

#endif // GAMEBOARDTOPCARD_H

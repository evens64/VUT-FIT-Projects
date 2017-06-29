#ifndef GAMEBOARDCARDSTACK_H
#define GAMEBOARDCARDSTACK_H

#include <QWidget>
#include <QGridLayout>
#include <QSignalMapper>
#include <array>
#include "Structs.h"
#include "CardButton.h"

/**
* \brief GUI element representing a stack of cards
*/

class GameBoardCardStack : public QWidget
{
	Q_OBJECT
public:
	explicit GameBoardCardStack(QWidget *parent, unsigned int stackgroup, unsigned int stack, const std::array<std::array<QIcon *,13>,5> &icons);
	void update(CardID newcards []);
	int getCardcount() const;
	void highlight(unsigned int pos,QString color);
	void unHighlight();
private:
	const std::array<std::array<QIcon *,13>,5> icons;
	CardButton *stackcards[19];
	unsigned int cardcount;
	QGridLayout *layout;
	bool inLayout [19];
	bool highlighted [19];
signals:

public slots:
};

#endif // GAMEBOARDCARDSTACK_H

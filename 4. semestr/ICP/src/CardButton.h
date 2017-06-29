#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QStylePainter>
#include <QStyleOptionButton>

/**
* \brief GUI element representing a card
*/
class CardButton : public QPushButton
{
public:
	CardButton(QWidget * parent);
	QSize sizeHint() const;
protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent * event);
	bool looptylooptd = false;
};

#endif // CARDBUTTON_H

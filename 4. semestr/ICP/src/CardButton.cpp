#include "CardButton.h"
/**
* \brief a constructor
*/
CardButton::CardButton(QWidget *parent): QPushButton(parent)
{
}
/**
* \brief redefinition of optimal element size
* \return optimal element's size
* returns width in 4/5 ratio to height, ratio of the card graphics
*/
QSize CardButton::sizeHint() const {
	return QSize(this->rect().height()*4/5, this->rect().height());
}

/**
* \brief redefinition of paint event
* \param[in] event incoming event
* paints the card with its icon
*/
void CardButton::paintEvent(QPaintEvent *event)
{
	event;
	QStylePainter painter(this);

	QStyleOptionButton options;
	this->initStyleOption(&options);

	options.iconSize = QSize(options.rect.width()-4, options.rect.height()-4);
	painter.drawControl(QStyle::CE_PushButton, options);
}
/**
* \brief redefinition of resize event
* \param[in] event incoming event
* updates geometry so that modified sizeHint can tak effect
*/
void CardButton::resizeEvent(QResizeEvent * event) {
	QPushButton::resizeEvent(event);
	if (!this->looptylooptd) {
		updateGeometry();
		looptylooptd = true;
		return;
	}
	this->looptylooptd = false;
}

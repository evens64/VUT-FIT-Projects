#include "GameBoardTopCard.h"
/**
* \brief A constructor
* \param[in] parent parent of this GUI element
* \param[in] stackgroup stackgroup of this stack, used while mapping signals
* \param[in] stack index of this stack within its group, used while mapping signals
* \param[in] icons icons for cards
*/
GameBoardTopCard::GameBoardTopCard(QWidget *parent, unsigned int stackgroup, unsigned int stack, const std::array<std::array<QIcon *,13>,5> &icons) : QWidget(parent), icons(icons)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	this->setLayout(layout);
	QSignalMapper* signalMapper = new QSignalMapper (this);

	this->card = new CardButton(this);
	card->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
	card->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Ignored);
	card->setBaseSize(20,25);
	card->setSizeIncrement(4,5);
	connect(this->card,SIGNAL(clicked()),signalMapper,SLOT(map()));
	signalMapper->setMapping (this->card,ClickID(stackgroup,stack,0).ToInt());
	layout->addWidget(this->card);
	layout->setAlignment(this->card,Qt::AlignHCenter);
	card->hide();
	connect(signalMapper,SIGNAL(mapped(int)),this->parent(),SLOT(onCardClick(int)));
}
/**
* \brief Highlights the card
* \param[in] color Color to highlight card with
*/
void GameBoardTopCard::highlight(QString color){
		this->card->setStyleSheet(QString("QPushButton {border-style: solid; border-width: 4px; border-color:")+color+QString(";}"));
		this->highlighted = true;
}

/**
* \brief Removes higlight from card
*/
void GameBoardTopCard::unHighlight(){
		if (this->highlighted){
			this->card->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
			this->highlighted = false;
		}
}

/**
* \brief Updates Graphics of the card
* \param[in] newcard card which this GUI element should display
*/
void GameBoardTopCard::update(CardID newcard){
	this->card->show();
	this->card->setIcon(*(this->icons[newcard.suit][newcard.value]));
	if(newcard.suit == 4 && newcard.value < 5) this->empty = true;
	else this->empty = false;
	this->unHighlight();
}
/**
* \brief a getter function
* \return True if stack is empty, false otherwise
*/

bool GameBoardTopCard::isEmpty() const{
	return this->empty;
}


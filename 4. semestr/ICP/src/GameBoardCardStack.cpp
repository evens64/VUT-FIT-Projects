#include "GameBoardCardStack.h"

/**
* \brief A constructor
* \param[in] parent parent of this GUI element
* \param[in] stackgroup stackgroup of this stack, used while mapping signals
* \param[in] stack index of this stack within its group, used while mapping signals
* \param[in] icons icons for cards
*/

GameBoardCardStack::GameBoardCardStack(QWidget *parent, unsigned int stackgroup, unsigned int stack,const std::array<std::array<QIcon *,13>,5> &icons) : QWidget(parent), icons(icons)
{
	this->layout = new QGridLayout;
	this->layout->setSpacing(0);
	this->layout->setContentsMargins(0,0,0,0);
	this->setLayout(this->layout);
	QSignalMapper* signalMapper = new QSignalMapper (this);
	for (unsigned int i = 0; i<19; i++){
		this->stackcards[i] = new CardButton(this);
		this->stackcards[i]->setBaseSize(20,25);
		this->stackcards[i]->setSizeIncrement(4,5);
		this->stackcards[i]->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
		this->stackcards[i]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Ignored);
		connect(this->stackcards[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
		signalMapper->setMapping (this->stackcards[i],ClickID(stackgroup,stack,i).ToInt());
		if (i<11){
			this->layout->addWidget(stackcards[i],i+1,1,4,1);
			this->inLayout[i] = true;
		}
		else this->inLayout[i] = false;
		stackcards[i]->hide();
		this->highlighted[i] = false;
	}
	connect(signalMapper,SIGNAL(mapped(int)),this->parent(),SLOT(onCardClick(int)));
}

/**
* \brief Highlights a portion of the stack
* \param[in] pos first highlighted card
* \param[in] color Color to highlight cards with
*/

void GameBoardCardStack::highlight(unsigned int pos,QString color){
	for(;pos<19;pos++){
		this->stackcards[pos]->setStyleSheet(QString("QPushButton {border-style: solid; border-width: 4px; border-color:")+color+QString(";}"));
		this->highlighted[pos] = true;
	}
}

/**
* \brief Removes higlight from stack
*/
void GameBoardCardStack::unHighlight(){
	for(int i=0;i<19;i++){
		if (this->highlighted[i]){
			this->stackcards[i]->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
			this->highlighted[i] = false;
		}
	}
}

/**
* \brief Updates Graphics of the cards
* \param[in] newcard cards which this GUI element should display
*/
void GameBoardCardStack::update(CardID newcards []){

	for (unsigned int i = 0; i<19; i++){
		if (newcards[i].suit == 4 && newcards[i].value == 6){ //žádná karta
			this->stackcards[i]->hide();
			if (i > 10 && inLayout[i]){
				this->layout->removeWidget(this->stackcards[i]);
				this->inLayout[i] = false;
			}
		}
		else{
			this->stackcards[i]->show();
			this->stackcards[i]->setIcon(*(this->icons[newcards[i].suit][newcards[i].value]));
			this->cardcount = i;
			this->stackcards[i]->setIconSize(this->stackcards[i]->size());
			if (i > 10 && !inLayout[i]){
				this->layout->addWidget(this->stackcards[i],i+1,1,4,1);
				this->inLayout[i] = true;
			}
		}
	}
	this->cardcount++;
	if(newcards[0].suit == 4 && newcards[0].value < 5) this->cardcount = 0;
	this->unHighlight();
}
/**
* \brief a getter function
* \return number of cards displayed in the stack
*/
int GameBoardCardStack::getCardcount() const{
	return this->cardcount;
}

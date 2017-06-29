// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#include "gameboard.h"


/**
* \brief constructor
*/
GameBoard::GameBoard(QWidget *parent, bool state,const std::array<std::array<QIcon *,13>,5> &icons) : QFrame(parent) {
	this->Build(icons);
	if (state == true) this->Activate(true); else this->Deactivate(true);
}

/**
* \brief destructor
*/
GameBoard::~GameBoard(){
	if (this->logicboard != NULL)
		delete this->logicboard;
}

/**
* \brief building gameboard
*/
void GameBoard::Build(const std::array<std::array<QIcon *,13>,5> &icons) {
	this->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
	/*this->setStyleSheet("QFrame { background-color: black; }"
						//"QPushButton { background-color: white; }"
						//"QMessageBox { background-color: gray; }"
						//"QMessageBox QLabel { background-color: gray; }"
						);*/

	QVBoxLayout *boardLayout = new QVBoxLayout;
	this->setLayout(boardLayout);

	//skóre
	QHBoxLayout *scoreLayout = new QHBoxLayout();
	QWidget *score = new QWidget();
	score->setObjectName("score");

	this->scoreLabel = new QLabel();
	//scoreLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	this->scoreLabel->setObjectName("scoreLabel");
	this->scoreLabel->setText("Skóre: 0");
	scoreLayout->addWidget(this->scoreLabel, 1);

	this->notificationLabel = new QLabel();
	//scoreLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	this->notificationLabel->setObjectName("notificationLabel");
	this->notificationLabel->setText("");
	scoreLayout->addWidget(this->notificationLabel, 2);

	this->notificationTimer = new QTimer(this);
	this->notificationTimer->setSingleShot(true);
	connect(this->notificationTimer, SIGNAL(timeout()),this,SLOT(clearNotification()));

	score->setLayout(scoreLayout);
	boardLayout->addWidget(score);

	//hrací plocha
	QFrame *gameFrame = new QFrame();
	QGridLayout *gameLayout = new QGridLayout;

	gameFrame->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
	gameFrame->setLayout(gameLayout);

	gameLayout->setSpacing(0);
	gameLayout->setContentsMargins(2,2,2,2);

	this->deckstack = new GameBoardTopCard(this,SolitaireBoard::deckGroup,0,icons);
	gameLayout->addWidget(this->deckstack,1,1,4,1);

	this->wastestack = new GameBoardTopCard(this,SolitaireBoard::drawGroup,0,icons);
	gameLayout->addWidget(this->wastestack,1,2,4,1);

	for (int i = 0; i<4; i++){
		this->foundationstacks[i] = new GameBoardTopCard(this,SolitaireBoard::foundationGroup,i,icons);
		gameLayout->addWidget(this->foundationstacks[i],1,i+4,4,1);
	}
	for (int i = 0; i<7; i++){
		this->pilestacks[i] = new GameBoardCardStack(this,SolitaireBoard::pileGroup,i,icons);
		gameLayout->addWidget(this->pilestacks[i],5,i+1,13,1);
	}

	boardLayout->addWidget(gameFrame, 13);

	//tlačítka
	QFrame *buttonFrame = new QFrame;
	QHBoxLayout *buttonLayout = new QHBoxLayout;

	QPushButton *btn_newGame = new QPushButton("Nová hra", this);
	QPushButton *btn_saveGame = new QPushButton("Uložit hru", this);
	QPushButton *btn_loadGame = new QPushButton("Načíst hru", this);
	QPushButton *btn_undoMove = new QPushButton("Tah zpět", this);
	QPushButton *btn_hintMove = new QPushButton("Nápověda tahu", this);
	QPushButton *btn_quitGame = new QPushButton("Ukončit hru", this);

	buttonLayout->addWidget(btn_newGame);
	buttonLayout->addWidget(btn_saveGame);
	buttonLayout->addWidget(btn_loadGame);
	buttonLayout->addWidget(btn_undoMove);
	buttonLayout->addWidget(btn_hintMove);
	buttonLayout->addWidget(btn_quitGame);


	buttonFrame->setLayout(buttonLayout);

	connect(btn_newGame,	SIGNAL(clicked()), this, SLOT(btnClick_newGame()));
	connect(btn_saveGame,	SIGNAL(clicked()), this, SLOT(btnClick_saveGame()));
	connect(btn_loadGame,	SIGNAL(clicked()), this, SLOT(btnClick_loadGame()));
	connect(btn_undoMove,	SIGNAL(clicked()), this, SLOT(btnClick_undoMove()));
	connect(btn_hintMove,	SIGNAL(clicked()), this, SLOT(btnClick_hintMove()));
	connect(btn_quitGame,	SIGNAL(clicked()), this, SLOT(btnClick_quitGame()));

	boardLayout->addWidget(buttonFrame, 1);


}

/// aktivace plochy
void GameBoard::Activate(bool doit) {
	if ( this->isActive() && doit == false) return;

	this->state = true;
	this->setDisabled(false);

	this->setStyleSheet("QFrame { background-color: green; }");

	this->logicboard = new GUISolitaireBoard;
	// ...
}

/// deaktivace plochy
void GameBoard::Deactivate(bool doit) {
	if ( !this->isActive() && doit == false ) return;

	this->state = false;
	this->setDisabled(true);

	this->setStyleSheet("QFrame { background-color: #f0f0f0; }");

	delete this->logicboard;
	this->logicboard = NULL;
	// ...
}

/// je aktivní?
bool GameBoard::isActive() {
	return this->state;
}
/**
* \brief displays a notification via label
* \param[in] text text to display
* notification lasts for 5 second, then disappears
*/
void GameBoard::notify(QString text){
	this->notificationLabel->setText(text);
	this->notificationTimer->start(5000);
}
/**
* \brief updates the GUI according to the internal logic
*/
void GameBoard::update(){
	this->deckstack->update(this->logicboard->GetTopCardID(SolitaireBoard::deckGroup,0));
	this->wastestack->update(this->logicboard->GetTopCardID(SolitaireBoard::drawGroup,0));
	for (int i = 0; i < 4; i++){
		this->foundationstacks[i]->update(this->logicboard->GetTopCardID(SolitaireBoard::foundationGroup,i));
	}
	for (int i = 0; i<7; i++){
		CardID arr[19];
		this->logicboard->GetStackIDs(SolitaireBoard::pileGroup,i,arr);
		this->pilestacks[i]->update(arr);
	}
	this->scoreLabel->setText(QString("Skóre:")+QString::number(this->logicboard->getScore()));
}
/**
* \brief removes all highlights on the board
*/
void GameBoard::unHighlightAll(){
	this->deckstack->unHighlight();
	this->wastestack->unHighlight();
	for(int i = 0;i<4;i++){
		this->foundationstacks[i]->unHighlight();
	}
	for(int i = 0;i<7;i++){
		this->pilestacks[i]->unHighlight();
	}
}
/**
* \brief clears displayed notification
*/
void GameBoard::clearNotification(){
	this->notificationLabel->setText("");
}

/**
* \brief slot for click on a card
* \param[in] intclck int representing a ClickID struct, eg. card which was clicked
* highlights and unhighlights clicked cards, constructs game moves to send to internal logic
*/
void GameBoard::onCardClick(int intclck){
	static ClickID lastclck = ClickID(0,0,0);
	ClickID clck = ClickID(intclck);
	//this->notify(QString("Card click :") + QString::number(clck.stackgroup) + QString(" ") + QString::number(clck.stack) + QString(" ") + QString::number(clck.stackpos));
	if (!this->cardselected){
		switch (clck.stackgroup){
			case SolitaireBoard::deckGroup:
				if(this->deckstack->isEmpty())
					this->logicboard->recycleDrawn();
				else
					this->logicboard->draw();
				this->update();
				return;
			case SolitaireBoard::drawGroup :
				this->move.mCardAmount = 1;
				this->wastestack->highlight("red");
				break;
			case SolitaireBoard::foundationGroup:
				this->move.mCardAmount = 1;
				this->foundationstacks[clck.stack]->highlight("red");
				break;
			case SolitaireBoard::pileGroup:
				this->move.mCardAmount = this->pilestacks[clck.stack]->getCardcount() - clck.stackpos;
				this->pilestacks[clck.stack]->highlight(clck.stackpos,"red");
		}
		this->move.mSourceGroup = (SolitaireBoard::CardStackGroups)clck.stackgroup;
		this->move.mSourceIndex = clck.stack;
		this->cardselected = true;
	}
	else{
		this->move.mTargetGroup = (SolitaireBoard::CardStackGroups)clck.stackgroup;
		this->move.mTargetIndex = clck.stack;
		this->cardselected = false;
		if(!this->logicboard->move(this->move)){
			this->notify("neplatný tah");
			this->unHighlightAll();
		}
		else this->update();
	}
}
/// tlačítka
/// nová hra
void GameBoard::btnClick_newGame() {
	this->logicboard->initialDeal();
	this->notify("Nová hra");
	this->update();
	//
}

/// uložit hru
void GameBoard::btnClick_saveGame() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Uložit hru"),
													QDir::currentPath(),
													tr("Uložené hry (*.sav)"));
	if (fileName.size() < 1){
		this->notify("Uložení neúspěšné");
		return;
	}
	this->logicboard->save(fileName.toLatin1().data());
	this->notify("Hra uložena");
	//
}

/// načíst hru
void GameBoard::btnClick_loadGame() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Načíst hru"),
													QDir::currentPath(),
													tr("Uložené hry (*.sav)"));
	if (fileName.size() < 1){
		this->notify("Nahrání neúspěšné");
		return;
	}
	if (this->logicboard->load(fileName.toLatin1().data()))
		this->notify("Hra načtena");
	else
		this->notify("Chyba při načítání hry");
	this->update();
	//
}


/// tah zpět
void GameBoard::btnClick_undoMove() {
	if (this->logicboard->undo()){
		this->update();
		this->notify("Tah vrácen");
	}
	else {
		this->notify("Žádné tahy k vrácení");
	}
	//
}

/// nápověda tahu
void GameBoard::btnClick_hintMove() {
	std::vector<SolitaireGameMove> hints = this->logicboard->getHints(3);
	if (hints.size() < 1){
		this->notify("Žádné návrhy");
		return;
	}
	const QString colors [3] = {"blue","yellow","cyan"}; //colors for hint highlights
	int i = 0;
	for (std::vector<SolitaireGameMove>::iterator iter = hints.begin(); iter != hints.end();++iter){
		switch ((*iter).mMoveType){
			case SolitaireGameMove::draw:
			case SolitaireGameMove::recycle:
				this->deckstack->highlight(colors[i]);
			break;
		case SolitaireGameMove::move:
			switch((*iter).mSourceGroup){
				case SolitaireBoard::drawGroup :
					this->wastestack->highlight(colors[i]);
				break;
				case SolitaireBoard::foundationGroup:
					this->foundationstacks[(*iter).mSourceIndex]->highlight(colors[i]);
				break;
				case SolitaireBoard::pileGroup:
					this->pilestacks[(*iter).mSourceIndex]->highlight(pilestacks[(*iter).mSourceIndex]->getCardcount() - (*iter).mCardAmount,colors[i]);
			}
			switch((*iter).mTargetGroup){
				case SolitaireBoard::drawGroup :
					this->wastestack->highlight(colors[i]);
				break;
				case SolitaireBoard::foundationGroup:
					this->foundationstacks[(*iter).mTargetIndex]->highlight(colors[i]);
				break;
				case SolitaireBoard::pileGroup:
					this->pilestacks[(*iter).mTargetIndex]->highlight(pilestacks[(*iter).mTargetIndex]->getCardcount()-1,colors[i]);
			}

		}
		i++;
	}
}

/// ukončit hru
void GameBoard::btnClick_quitGame() {
	this->logicboard->clearBoard();
	this->notify("Hra ukončena");
	this->update();
	//
}

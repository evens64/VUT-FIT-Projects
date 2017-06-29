// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#include "mainwindow.h"
#include "ui_mainwindow.h"

/// constructor
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->createicons();
}

/// destructor
MainWindow::~MainWindow()
{
	delete ui;
}

/// ukázat 'o hře'
void MainWindow::sl_displayAbout()
{
	QMessageBox::about(this,
					   "O hře",
					   "Karetní hra Solitaire\n"
					   "made by Jakub Frýz & Michal Hrabovský"
					   );
}

/// nová hra
void MainWindow::sl_newGame()
{
	NewGameDialog *dialog = new NewGameDialog(0, this->games[0], this->games[1], this->games[2], this->games[3]);
	dialog->exec();

	if ( dialog->result() == QDialog::Accepted ) {
		this->setGames( dialog->getBoolValue(1), dialog->getBoolValue(2), dialog->getBoolValue(3), dialog->getBoolValue(4) );
	}

	this->manageBoards();
}

/// gameboards management
void MainWindow::manageBoards() {
	switch (this->getGamesCount()) {
		case 0:
			if ( this->getBoardsCount() == 0 ) return;

			if ( this->existsBoard(0) ) {
				this->boards[0]->close();
				ui->gridLayout->removeWidget(this->boards[0]);
				this->boards[0]->destroyed();
				this->boards[0] = NULL;
			}
			if ( this->existsBoard(1) ) {
				this->boards[1]->close();
				ui->gridLayout->removeWidget(this->boards[1]);
				this->boards[1]->destroyed();
				this->boards[1] = NULL;
			}
			if ( this->existsBoard(2) ) {
				this->boards[2]->close();
				ui->gridLayout->removeWidget(this->boards[2]);
				this->boards[2]->destroyed();
				this->boards[2] = NULL;
			}
			if ( this->existsBoard(3) ) {
				this->boards[3]->close();
				ui->gridLayout->removeWidget(this->boards[3]);
				this->boards[3]->destroyed();
				this->boards[3] = NULL;
			}

			break;

		case 1:
			// hry , které mají být odstraněny
			if ( this->games[0] == false && this->existsBoard(0) ) {
				this->boards[0]->close();
				ui->gridLayout->removeWidget(this->boards[0]);
				this->boards[0]->destroyed();
				this->boards[0] = NULL;
			}
			if ( this->games[1] == false && this->existsBoard(1) ) {
				this->boards[1]->close();
				ui->gridLayout->removeWidget(this->boards[1]);
				this->boards[1]->destroyed();
				this->boards[1] = NULL;
			}
			if ( this->games[2] == false && this->existsBoard(2) ) {
				this->boards[2]->close();
				ui->gridLayout->removeWidget(this->boards[2]);
				this->boards[2]->destroyed();
				this->boards[2] = NULL;
			}
			if ( this->games[3] == false && this->existsBoard(3) ) {
				this->boards[3]->close();
				ui->gridLayout->removeWidget(this->boards[3]);
				this->boards[3]->destroyed();
				this->boards[3] = NULL;
			}

			// pokud není žádná hra aktivní
			if ( this->getBoardsCount() == 0 ) {
				if ( this->games[0] == true ) {
					this->boards[0] = new GameBoard(this, true, this->icons);
					ui->gridLayout->addWidget(this->boards[0], 0, 0);
				} else if ( this->games[1] == true ) {
					this->boards[1] = new GameBoard(this, true, this->icons);
					ui->gridLayout->addWidget(this->boards[1], 0, 0);
				} else if ( this->games[2] == true ) {
					this->boards[2] = new GameBoard(this, true, this->icons);
					ui->gridLayout->addWidget(this->boards[2], 0, 0);
				} else if ( this->games[3] == true ) {
					this->boards[3] = new GameBoard(this, true, this->icons);
					ui->gridLayout->addWidget(this->boards[3], 0, 0);
				}
			} else {
				// pokud hra existuje ale možná není aktivní
				if ( this->existsBoard(0) ) {
					if ( !this->boards[0]->isActive() ) this->boards[0]->Activate();
				} else if ( this->existsBoard(1) ) {
					if ( !this->boards[1]->isActive() ) this->boards[1]->Activate();
				} else if ( this->existsBoard(2) ) {
					if ( !this->boards[2]->isActive() ) this->boards[2]->Activate();
				} else if ( this->existsBoard(3) ) {
					if ( !this->boards[3]->isActive() ) this->boards[3]->Activate();
				}
			}

			break;

		case 2:
		case 3:
		case 4:

			// pokud je jen jedna aktivní plocha
			if ( this->getBoardsCount() == 1 ) {
				if ( this->existsBoard(0) ) {
					ui->gridLayout->removeWidget(this->boards[0]);
				} else if ( this->existsBoard(1) ) {
					ui->gridLayout->removeWidget(this->boards[1]);
				} else if ( this->existsBoard(2) ) {
					ui->gridLayout->removeWidget(this->boards[2]);
				} else if ( this->existsBoard(3) ) {
					ui->gridLayout->removeWidget(this->boards[3]);
				}
			}

			// vytvoření zbylých ploch
			if ( !this->existsBoard(0) )
				this->boards[0] = new GameBoard(this, this->games[0], this->icons);
			else if ( !this->boards[0]->isActive() && this->games[0] == true )
				this->boards[0]->Activate();
			else if ( this->boards[0]->isActive() && this->games[0] == false )
				this->boards[0]->Deactivate();

			if ( !this->existsBoard(1) )
				this->boards[1] = new GameBoard(this, this->games[1], this->icons);
			else if ( !this->boards[1]->isActive() && this->games[1] == true )
				this->boards[1]->Activate();
			else if ( this->boards[1]->isActive() && this->games[1] == false )
				this->boards[1]->Deactivate();

			if ( !this->existsBoard(2) )
				this->boards[2] = new GameBoard(this, this->games[2], this->icons);
			else if ( !this->boards[2]->isActive() && this->games[2] == true )
				this->boards[2]->Activate();
			else if ( this->boards[2]->isActive() && this->games[2] == false )
				this->boards[2]->Deactivate();

			if ( !this->existsBoard(3) )
				this->boards[3] = new GameBoard(this, this->games[3], this->icons);
			else if ( !this->boards[3]->isActive() && this->games[3] == true )
				this->boards[3]->Activate();
			else if ( this->boards[3]->isActive() && this->games[3] == false )
				this->boards[3]->Deactivate();


			ui->gridLayout->addWidget(this->boards[0], 0, 0);
			ui->gridLayout->addWidget(this->boards[1], 0, 1);
			ui->gridLayout->addWidget(this->boards[2], 1, 0);
			ui->gridLayout->addWidget(this->boards[3], 1, 1);

			break;

		default:
			break;
	}
}

/// set games values
void MainWindow::setGames(bool g01, bool g02, bool g03, bool g04) {
	this->games[0] = g01;
	this->games[1] = g02;
	this->games[2] = g03;
	this->games[3] = g04;
}

/// get number of active games
int MainWindow::getGamesCount() {
	int count = 0;

	for (int i = 0; i < 4; i++) {
		if ( this->games[i] == true ) count++;
	}

	return count;
}

/// get number of active boards
int MainWindow::getBoardsCount() {
	int count = 0;

	for (int i = 0; i < 4; i++) {
		if ( this->boards[i] != NULL ) count++;
	}

	return count;
}

/// check if board exists
bool MainWindow::existsBoard(int board) {
	if ( board >= 0 && board <= 3) {
		return this->boards[board] != NULL ? true : false;
	} else return false;
}

/// create all icons for game
void MainWindow::createicons() {

	this->icons[4][0] = new QIcon(":/piles/img/pile_heart.png");
	this->icons[4][1] = new QIcon(":/piles/img/pile_diamond.png");
	this->icons[4][2] = new QIcon(":/piles/img/pile_club.png");
	this->icons[4][3] = new QIcon(":/piles/img/pile_spade.png");
	this->icons[4][4] = new QIcon(":/piles/img/card_empty_stack.png");
	this->icons[4][5] = new QIcon(":/back/img/card_back.png");

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			switch (i) {
			case 0:				// hearts
				switch (j) {

				case 0:				// A
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_A.png");
					break;

				case 1:				// 2
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_2.png");
					break;

				case 2:				// 3
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_3.png");
					break;

				case 3:				// 4
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_4.png");
					break;

				case 4:				// 5
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_5.png");
					break;

				case 5:				// 6
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_6.png");
					break;

				case 6:				// 7
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_7.png");
					break;

				case 7:				// 8
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_8.png");
					break;

				case 8:				// 9
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_9.png");
					break;

				case 9:				// 10
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_10.png");
					break;

				case 10:			// J
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_J.png");
					break;

				case 11:			// Q
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_Q.png");
					break;

				case 12:			// K
					this->icons[i][j] = new QIcon(":/cards/img/card_heart_K.png");
					break;

				}
				break;

			case 1:				// diamonds
				switch (j) {

				case 0:				// A
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_A.png");
					break;

				case 1:				// 2
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_2.png");
					break;

				case 2:				// 3
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_3.png");
					break;

				case 3:				// 4
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_4.png");
					break;

				case 4:				// 5
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_5.png");
					break;

				case 5:				// 6
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_6.png");
					break;

				case 6:				// 7
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_7.png");
					break;

				case 7:				// 8
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_8.png");
					break;

				case 8:				// 9
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_9.png");
					break;

				case 9:				// 10
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_10.png");
					break;

				case 10:			// J
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_J.png");
					break;

				case 11:			// Q
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_Q.png");
					break;

				case 12:			// K
					this->icons[i][j] = new QIcon(":/cards/img/card_diamond_K.png");
					break;

				}
				break;

			case 2:				// clubs
				switch (j) {

				case 0:				// A
					this->icons[i][j] = new QIcon(":/cards/img/card_club_A.png");
					break;

				case 1:				// 2
					this->icons[i][j] = new QIcon(":/cards/img/card_club_2.png");
					break;

				case 2:				// 3
					this->icons[i][j] = new QIcon(":/cards/img/card_club_3.png");
					break;

				case 3:				// 4
					this->icons[i][j] = new QIcon(":/cards/img/card_club_4.png");
					break;

				case 4:				// 5
					this->icons[i][j] = new QIcon(":/cards/img/card_club_5.png");
					break;

				case 5:				// 6
					this->icons[i][j] = new QIcon(":/cards/img/card_club_6.png");
					break;

				case 6:				// 7
					this->icons[i][j] = new QIcon(":/cards/img/card_club_7.png");
					break;

				case 7:				// 8
					this->icons[i][j] = new QIcon(":/cards/img/card_club_8.png");
					break;

				case 8:				// 9
					this->icons[i][j] = new QIcon(":/cards/img/card_club_9.png");
					break;

				case 9:				// 10
					this->icons[i][j] = new QIcon(":/cards/img/card_club_10.png");
					break;

				case 10:			// J
					this->icons[i][j] = new QIcon(":/cards/img/card_club_J.png");
					break;

				case 11:			// Q
					this->icons[i][j] = new QIcon(":/cards/img/card_club_Q.png");
					break;

				case 12:			// K
					this->icons[i][j] = new QIcon(":/cards/img/card_club_K.png");
					break;

				}
				break;

			case 3:				// spades
				switch (j) {

				case 0:				// A
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_A.png");
					break;

				case 1:				// 2
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_2.png");
					break;

				case 2:				// 3
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_3.png");
					break;

				case 3:				// 4
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_4.png");
					break;

				case 4:				// 5
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_5.png");
					break;

				case 5:				// 6
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_6.png");
					break;

				case 6:				// 7
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_7.png");
					break;

				case 7:				// 8
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_8.png");
					break;

				case 8:				// 9
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_9.png");
					break;

				case 9:				// 10
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_10.png");
					break;

				case 10:			// J
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_J.png");
					break;

				case 11:			// Q
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_Q.png");
					break;

				case 12:			// K
					this->icons[i][j] = new QIcon(":/cards/img/card_spade_K.png");
					break;

				}
				break;
			}
		}
	}

}

// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QtWidgets>
#include <array>

#include "newgamedialog.h"
#include "gameboard.h"
#include "Card.h"


namespace Ui {
	class MainWindow;
}

/**
* \brief main window of GUI
*/

class MainWindow : public QMainWindow
{
	Q_OBJECT

public slots:
	void sl_displayAbout();
	void sl_newGame();

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void manageBoards();
	void setGames(bool, bool, bool, bool);
	int getGamesCount();
	int getBoardsCount();
	bool existsBoard(int);

	void createicons();

private:
	Ui::MainWindow *ui;
	bool games[4] = {false, false, false, false};
	GameBoard *boards[4] = {NULL, NULL, NULL, NULL};

	std::array<std::array<QIcon *,13>,5> icons;

};


#endif // MAINWINDOW_H

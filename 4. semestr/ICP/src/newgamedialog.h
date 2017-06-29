// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "mainwindow.h"


namespace Ui {
	class NewGameDialog;
}

/**
* \brief Supporting dialog window
*/

class NewGameDialog : public QDialog
{
	Q_OBJECT

public slots:
	void sl_buttonName();

public:
	NewGameDialog(QWidget *parent = 0, bool game01 = false, bool game02 = false, bool game03 = false, bool game04 = false);
	~NewGameDialog();
	bool getBoolValue(int button);

private:
	Ui::NewGameDialog *ui;

};

#endif // NEWGAMEDIALOG_H

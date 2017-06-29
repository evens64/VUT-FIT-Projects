// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#include "newgamedialog.h"
#include "ui_newgamedialog.h"


NewGameDialog::NewGameDialog(QWidget *parent, bool game01, bool game02, bool game03, bool game04) :
	QDialog(parent),
	ui(new Ui::NewGameDialog)
{
	ui->setupUi(this);

	ui->pushButton_1->setChecked(game01);
	ui->pushButton_2->setChecked(game02);
	ui->pushButton_3->setChecked(game03);
	ui->pushButton_4->setChecked(game04);
}

NewGameDialog::~NewGameDialog()
{
	delete ui;
}

/// mění názvy na tlačítkách
void NewGameDialog::sl_buttonName() {
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if( button != NULL ) {
		if ( button->objectName() == "pushButton_1" ) {
			if ( button->isChecked() ) {
				button->setText("01: aktivní");
			} else {
				button->setText("01: neaktivní");
			}
		} else if ( button->objectName() == "pushButton_2" ) {
			if ( button->isChecked() ) {
				button->setText("02: aktivní");
			} else {
				button->setText("02: neaktivní");
			}
		} else if ( button->objectName() == "pushButton_3" ) {
			if ( button->isChecked() ) {
				button->setText("03: aktivní");
			} else {
				button->setText("03: neaktivní");
			}
		} else if ( button->objectName() == "pushButton_4" ) {
			if ( button->isChecked() ) {
				button->setText("04: aktivní");
			} else {
				button->setText("04: neaktivní");
			}
		}
	}
}

///vrátí hodnoty tlačítek
bool NewGameDialog::getBoolValue(int button) {
	switch (button) {
		case 1:
			return ui->pushButton_1->isChecked();
			break;

		case 2:
			return ui->pushButton_2->isChecked();
			break;

		case 3:
			return ui->pushButton_3->isChecked();
			break;

		case 4:
			return ui->pushButton_4->isChecked();
			break;

		default:
			return false;
	}
}

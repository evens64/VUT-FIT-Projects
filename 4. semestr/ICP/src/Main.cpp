// Autoři: Michal Hrabovský (xhrabo08) & Jakub Frýz (xfryzj01)

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication game(argc, argv);
	QCoreApplication::setApplicationName("HRA2017");
	QCoreApplication::setOrganizationName("VUT-FIT");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	MainWindow window;
	window.showMaximized();

	return game.exec();
}

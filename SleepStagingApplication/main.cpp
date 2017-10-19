#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "Model.h"
#include "Controller.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow *mainWindow = new MainWindow;
	mainWindow->show();
	Model *model = new Model;
	Controller *controller = new Controller(mainWindow, model);
	
	return a.exec();
}

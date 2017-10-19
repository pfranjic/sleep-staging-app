#pragma once

#include <QWidget>
#include "ui_EDFPopUp.h"
#include <QtWidgets>

class EDFPopUp : public QWidget
{
	Q_OBJECT

public:
	EDFPopUp(QWidget *parent = Q_NULLPTR);
	~EDFPopUp();
	QPushButton *OKButton;
	QLineEdit *epochDurationLineEdit;
	QLineEdit *fsLineEdit;

private:
	Ui::EDFPopUp ui;
	QVBoxLayout *mainLayout;

};

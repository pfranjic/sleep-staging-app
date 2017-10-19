#include "EDFPopUp.h"
#include "MainWindow.h"

EDFPopUp::EDFPopUp(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->resize(175, 150);
	epochDurationLineEdit = new QLineEdit;
	epochDurationLineEdit->setFixedWidth(50);
	fsLineEdit = new QLineEdit;
	fsLineEdit->setFixedWidth(50);

	QLabel *epochDurationLabel = new QLabel(tr("Epoch duration (s):"));
	QLabel *fsLabel = new QLabel(tr("Sampling frequency (Hz):"));
	OKButton = new QPushButton("OK");

	QGridLayout *EDFInfoLayout = new QGridLayout;
	EDFInfoLayout->addWidget(epochDurationLabel, 0, 0);
	EDFInfoLayout->addWidget(epochDurationLineEdit, 0, 1);
	EDFInfoLayout->addWidget(fsLabel, 1, 0);
	EDFInfoLayout->addWidget(fsLineEdit, 1, 1);
	EDFInfoLayout->addWidget(OKButton, 2, 0, 1, 2);
	this->setLayout(EDFInfoLayout);
	setWindowTitle(tr("EDF File Information"));


}

EDFPopUp::~EDFPopUp()
{
}

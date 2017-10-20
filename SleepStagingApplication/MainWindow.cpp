#include "MainWindow.h"

using namespace std;
using namespace boost;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	createComponents();

	layoutComponents();

	connect(dataEndEpochLineEdit, &QLineEdit::editingFinished, this, &MainWindow::markEpochInterval);
	connect(extractFeaturesButton, &QPushButton::clicked, this, &MainWindow::markEpochInterval);
	connect(clearOutputButton, &QPushButton::clicked, this, &MainWindow::clearOutput);
	connect(trainMLAlgorithmButton, &QPushButton::clicked, this, &MainWindow::sendMLAlgorithmInfoSignal);
	connect(MLAlgorithmsComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), classifiersSetup, &QStackedWidget::setCurrentIndex);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createComponents() {
	mainWidget = new QWidget;
	mainLayout = new QVBoxLayout;

	openAnnotationFileButton = new QPushButton("Open annotation file");
	openEDFFileButton = new QPushButton("Open EDF file");
	generatePlotButton = new QPushButton("Generate hypnogram");
	fileLayout = new QVBoxLayout;
	fileGroup = new QGroupBox("File opening && plot generation");

	dataSettingLayout = new QVBoxLayout;
	dataSelectionGroup = new QGroupBox("Data selection");
	extractFeaturesButton = new QPushButton("Extract features");
	featuresList = new QListWidget;
	featuresList->setSelectionMode(QAbstractItemView::MultiSelection);
	QListWidgetItem *psdItem = new QListWidgetItem("<< PSD >>", featuresList);
	psdItem->setFlags(psdItem->flags() & ~Qt::ItemIsSelectable);
	psdItem->setTextAlignment(Qt::AlignHCenter);
	QListWidgetItem *alpha = new QListWidgetItem("alpha/total", featuresList);
	//alpha->setTextAlignment(Qt::AlignRight);
	QListWidgetItem *beta = new QListWidgetItem("beta/total", featuresList);
	//beta->setTextAlignment(Qt::AlignRight);
	QListWidgetItem *delta = new QListWidgetItem("delta/total", featuresList);
	//delta->setTextAlignment(Qt::AlignRight);
	QListWidgetItem *theta = new QListWidgetItem("theta/total", featuresList);
	//theta->setTextAlignment(Qt::AlignRight);
	dataStartEpochLabel = new QLabel("Starting epoch:");
	dataStartEpochLineEdit = new QLineEdit;
	dataEndEpochLabel = new QLabel("Ending epoch:");
	dataEndEpochLineEdit = new QLineEdit;
	featureSelectionLayout = new QVBoxLayout;
	featureSelectionGroup = new QGroupBox("Feature selection");
	featuresLayout = new QHBoxLayout;
	featuresGroup = new QGroupBox("Feature extraction");

	MLAlgorithmsComboBox = new QComboBox;
	MLAlgorithmsComboBox->addItem("LDA", "LDA");
	MLAlgorithmsComboBox->addItem("SVM", "SVM");
	MLGroup = new QGroupBox("Learning algorithm && parameters");
	SVMCText = new TextIdQLineEdit;
	SVMCText->setTextId("C");
	SVMGammaText = new TextIdQLineEdit;
	SVMGammaText->setTextId("gamma");
	biasCheckBox = new TextIdQCheckBox("Bias");
	biasCheckBox->setTextId("bias");
	SVMOptionsLayout = new QVBoxLayout;
	SVMCTextLabel = new QLabel("C:");
	SVMGammaTextLabel = new QLabel("Gamma:");
	MLLayout = new QVBoxLayout;
	trainMLAlgorithmButton = new QPushButton("Train ML algorithm");
	LDASetupWidget = new QWidget;
	SVMSetupWidget = new QWidget;
	classifiersSetup = new QStackedWidget;
	testPercentageText = new TextIdQLineEdit;
	testPercentageText->setValidator(new QIntValidator(1, 100, testPercentageText));
	testPercentageLabel = new QLabel("% of data used for testing");

	outputText = new QPlainTextEdit;
	clearOutputButton = new QPushButton("Clear");
	outputLayout = new QVBoxLayout;
	outputGroup = new QGroupBox("Algorithm output");
	
	controlsLayout = new QHBoxLayout;
	controlsGroup = new QGroupBox("Controls");

	plotLayout = new QVBoxLayout;
	plotGroup = new QGroupBox("Hypnogram");
	plotGroup->setVisible(false);
	plot = new QCustomPlot();
	plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight | Qt::AlignBottom);
}

void MainWindow::layoutComponents() {
	fileLayout->addWidget(openAnnotationFileButton);
	fileLayout->addWidget(openEDFFileButton);
	fileLayout->addWidget(generatePlotButton);
	fileGroup->setLayout(fileLayout);

	dataSettingLayout->addWidget(dataStartEpochLabel);
	dataSettingLayout->addWidget(dataStartEpochLineEdit);
	dataSettingLayout->addWidget(dataEndEpochLabel);
	dataSettingLayout->addWidget(dataEndEpochLineEdit);
	dataSelectionGroup->setLayout(dataSettingLayout);

	featureSelectionLayout->addWidget(featuresList);
	featureSelectionGroup->setLayout(featureSelectionLayout);

	featuresLayout->addWidget(dataSelectionGroup);
	featuresLayout->addWidget(featureSelectionGroup);
	featuresLayout->addWidget(extractFeaturesButton);
	featuresGroup->setLayout(featuresLayout);

	SVMOptionsLayout->addWidget(SVMCTextLabel);
	SVMOptionsLayout->addWidget(SVMCText);
	SVMOptionsLayout->addWidget(SVMGammaTextLabel);
	SVMOptionsLayout->addWidget(SVMGammaText);
	SVMOptionsLayout->addWidget(biasCheckBox);
	SVMSetupWidget->setLayout(SVMOptionsLayout);

	classifiersSetup->addWidget(LDASetupWidget);
	classifiersSetup->addWidget(SVMSetupWidget);

	MLLayout->addWidget(MLAlgorithmsComboBox);
	MLLayout->addWidget(classifiersSetup);
	MLLayout->addWidget(testPercentageLabel);
	MLLayout->addWidget(testPercentageText);
	MLLayout->addWidget(trainMLAlgorithmButton);
	MLGroup->setLayout(MLLayout);

	outputLayout->addWidget(outputText);
	outputLayout->addWidget(clearOutputButton);
	outputGroup->setLayout(outputLayout);

	controlsLayout->addWidget(fileGroup);
	controlsLayout->addWidget(featuresGroup);
	controlsLayout->addWidget(MLGroup);
	controlsLayout->addWidget(outputGroup);
	controlsGroup->setLayout(controlsLayout);

	plotLayout->addWidget(plot);
	plotGroup->setLayout(plotLayout);

	mainLayout->addWidget(controlsGroup);
	mainLayout->addWidget(plotGroup);
	mainWidget->setLayout(mainLayout);
	setWindowTitle("Sleep staging");

	setCentralWidget(mainWidget);
}

void MainWindow::generatePlot(QVector<double> annotations)
{
	QVector<double> x;
	for (int ind = 0; ind < annotations.size(); ind++) {
		x.push_back(ind);
	}
	plot->setInteraction(QCP::iRangeDrag, true);
	plot->setInteraction(QCP::iRangeZoom, true);
	plot->addGraph();
	QString name = QString::number(0);
	plot->addLayer(name);
	plot->graph(0)->setData(x, annotations);
	plot->xAxis->setLabel("Epoch number");
	plot->yAxis->setLabel("Sleep stage");
	plot->xAxis->setRange(0, 3000);
	plot->yAxis->setRange(0, 8);
	QVector<double> ticks;
	QVector<QString> labels;
	ticks << 1 << 2 << 3 << 4 << 5 << 6;
	labels << "S4" << "S3" <<  "S2" << "S1" << "REM" << "Awake";
	QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
	textTicker->addTicks(ticks, labels);
	plot->yAxis->setTicker(textTicker);

	plot->graph(0)->setLayer(name);
	plot->legend->removeItem(plot->legend->itemCount()-1);
	plot->replot();
	plot->axisRect()->setMinimumSize(150, 150);
	plotGroup->setVisible(true);
}

QString MainWindow::getAnnotationFileName()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Text File"), "",
		tr("Text File (*.txt);;All Files (*)"));
	return fileName;
}

QString MainWindow::getEDFFileName()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open EDF File"), "",
		tr("EDF File (*.edf);;All Files (*)"));
	return fileName;
}

void MainWindow::clearOutput() {
	outputText->clear();
}

void MainWindow::markEpochInterval() {
	int xEnd = dataEndEpochLineEdit->text().toInt();
	int xStart = dataStartEpochLineEdit->text().toInt();
	if (xEnd > xStart) {
			QString name = QString::number(1);
			plot->addLayer(name);
			plot->addGraph();
			if (plot->graphCount() != 2) {
				plot->legend->removeItem(plot->legend->itemCount() - 1);
			}
			plot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 127), Qt::SolidPattern));
			plot->graph(1)->setPen(QPen(QColor(255, 0, 0, 127)));
			plot->graph(1)->setName("Selected data");
			QVector<double> x, y;

			for (int ind = xStart; ind <= xEnd; ind++) {
				x.push_back(ind);
				y.push_back(8); //max y
			}
			plot->graph(1)->setData(x, y);
			plot->graph(1)->setLayer(name);
			QCPLayer *selectionLayer = plot->layer(QString::number(1));
			QCPLayer *epochsLayer = plot->layer(QString::number(0));
			if (selectionLayer && epochsLayer)
				plot->moveLayer(selectionLayer, epochsLayer, QCustomPlot::limBelow);
			plot->legend->setVisible(true);
			plot->replot();


	}
	else {
		QToolTip::showText(dataEndEpochLineEdit->mapToGlobal(QPoint()), tr("End epoch must be larger than start epoch"));

	}

}

void MainWindow::setAnnotationValidators(int annotations_size) {
	dataStartEpochLineEdit->setValidator(new QIntValidator(1, annotations_size, this));
	dataStartEpochLineEdit->setValidator(new QIntValidator(1, annotations_size, this));
}

void MainWindow::showEDFInfoPopUp() {
	EDFpopUp = new EDFPopUp();
	connect(EDFpopUp->OKButton, &QPushButton::clicked, this, &MainWindow::EDFFileInfoSet);

	EDFpopUp->show();
}

void MainWindow::EDFFileInfoSet() {
	double epochDuration = EDFpopUp->epochDurationLineEdit->text().toDouble();
	double fs = EDFpopUp->fsLineEdit->text().toDouble();
	emit EDFFileInfoSignal(epochDuration, fs);
	EDFpopUp->close();

}

void MainWindow::printAlgorithmResults(pair<mat, float> results, map<int, string> phaseMapping) {
	outputText->appendPlainText("RESULTS: \n========");
	map<string, string> nameMappings;
	nameMappings["Sleep_stage_W"] = "Awake";
	nameMappings["Sleep_stage_1"] = "S1";
	nameMappings["Sleep_stage_2"] = "S2";
	nameMappings["Sleep_stage_3"] = "S3";
	nameMappings["Sleep_stage_4"] = "S4";
	nameMappings["Sleep_stage_R"] = "REM";
	std::stringstream ss;
	//ss << std::setw(3);
	int nr_classes = 6;
	mat precisionRecall = results.first;
	ss << format("%8s%15s%13s\n") % "Phase" % "Precision %" % "Recall %";

	for (int row = 0; row < precisionRecall.n_rows; row++) {
		int col_sum = 0;
		ss << format("%8s") % nameMappings[phaseMapping[row + 1]];
		for (int col = 0; col < precisionRecall.n_cols; col++) {
			if (~isnan(precisionRecall(row, col))) {
				ss << format("%16.2f") % (precisionRecall(row, col) * 100);
			}
			else {
				ss << format("%18s") % "NaN";
			}
		}
		ss << "\n";
	}
	ss << format("\nAccuracy: %10.2f %%") % (results.second * 100);
	outputText->appendPlainText(QString::fromStdString(ss.str()));
}

void MainWindow::notifyFeaturesExtracted() {
	outputText->appendPlainText("Features extracted! ");
}

QSet<QString> MainWindow::getSelectedFeatures() 
{
	QList<QListWidgetItem *> selectedFeaturesList = featuresList->selectedItems();
	QSet<QString> selectedFeaturesSet;
	for (int i = 0; i < selectedFeaturesList.size(); ++i) {
		selectedFeaturesSet.insert(selectedFeaturesList.at(i)->text());
	}
	return selectedFeaturesSet;
}

void MainWindow::sendMLAlgorithmInfoSignal() {
	if (!testPercentageText->text().isEmpty()) {
		map <string, string> algorithmInfo;
		algorithmInfo.insert(pair <string, string>("name", MLAlgorithmsComboBox->currentText().toStdString()));
		algorithmInfo.insert(pair <string, string>("testPercentage", testPercentageText->text().toStdString()));
		QLayout *currentLayout = classifiersSetup->currentWidget()->layout();
		if (currentLayout != NULL) {
			for (int i = 0; i < currentLayout->count(); ++i)
			{
				QWidget *widget = currentLayout->itemAt(i)->widget();
				if (dynamic_cast<TextId*>(widget))
				{
					algorithmInfo.insert(pair <string, string>(dynamic_cast<TextId*>(widget)->getTextId().toStdString(),
						dynamic_cast<TextId*>(widget)->getValue().toStdString()));
				}
				else
				{
				}
			}
		}

		emit MLAlgorithmInfoSignal(algorithmInfo);
	}
	else {
		QToolTip::showText(testPercentageText->mapToGlobal(QPoint()), tr("Field cannot be empty!"));
	}
 
}



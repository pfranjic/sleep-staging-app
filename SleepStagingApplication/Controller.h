#pragma once

#include "MainWindow.h"
#include "Model.h"
#include <QObject>

class Controller : public QObject
{
	Q_OBJECT
	MainWindow *mMainWindow;
	Model *mModel;
public:
	Controller(MainWindow *mainWindow, Model *model);
	~Controller();
public slots:
	void openAnnotationFile();
	void openEDFFile();
	void loadEpochs(double epochDuration, double fs);
	void generatePlot();
	void setEpochInterval();
	void extractFeatures();
	void runMLAlgorithm(map<string, string> algorithmInfo);
signals:
	void MLAlgorithmRunSignal(pair<mat, float> results, map<int, string> phaseMapping);
	void featuresExtractedSignal();
};


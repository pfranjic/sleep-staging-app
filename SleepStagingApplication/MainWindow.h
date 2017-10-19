#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include <QWidget>
#include "ui_MainWindow.h"
#include "qcustomplot.h"
#include "EDFPopUp.h"
#include <memory>
#include <boost/format.hpp>
#ifndef _MAT_
#define _MAT_
#include <armadillo>
#include "mat.h"
#include "mex.h"
#include "armaMex.hpp"
#endif
#include <sstream>
#include "TextIdQLineEdit.h"
#include "TextIdQCheckBox.h"
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
	QString getAnnotationFileName();
	QString getEDFFileName();
	public slots:
	void generatePlot(QVector<double> annotations);
	void markEpochInterval();
	void clearOutput();
	void EDFFileInfoSet();
	void printAlgorithmResults(pair<mat, float> results, map<int, string> phaseMapping);
	void notifyFeaturesExtracted();
	void sendMLAlgorithmInfoSignal();
	signals:
	void EDFFileInfoSignal(double epochDuration, double fs);
	void MLAlgorithmInfoSignal(map<string, string> algorithmInfo);
public:
	Ui::MainWindow ui;
	QString curFile;
	QWidget *mainWidget;
	QCustomPlot *plot;
	QPushButton *generatePlotButton;
	QPushButton *openAnnotationFileButton;
	QPushButton *extractFeaturesButton;
	QPushButton *openEDFFileButton;
	QListWidget *featuresList;
	QComboBox *MLAlgorithmsComboBox;
	TextIdQLineEdit *SVMCText;
	TextIdQLineEdit *SVMGammaText;
	TextIdQCheckBox *biasCheckBox;
	QLabel* SVMCTextLabel;
	QLabel* SVMGammaTextLabel;
	QVBoxLayout *SVMOptionsLayout;
	TextIdQLineEdit *testPercentageText;
	QLabel *testPercentageLabel;
	QPushButton *trainMLAlgorithmButton;
	QPlainTextEdit *outputText;
	QGroupBox *fileGroup;
	QGroupBox *featuresGroup;
	QVBoxLayout *fileLayout;
	QHBoxLayout *controlsLayout;
	QHBoxLayout *featuresLayout;
	QVBoxLayout *mainLayout;
	QVector<double> annotations;
	QLineEdit *dataStartEpochLineEdit;
	QLineEdit *dataEndEpochLineEdit;
	QLabel* dataStartEpochLabel;
	QLabel* dataEndEpochLabel;
	QVBoxLayout *dataSettingLayout;
	QVBoxLayout *MLLayout;
	QGroupBox *MLGroup;
	QGroupBox *dataSelectionGroup;
	QGroupBox *controlsGroup;
	EDFPopUp *EDFpopUp;
	QWidget *LDASetupWidget;
	QWidget *SVMSetupWidget;
	QStackedWidget *classifiersSetup;
	void setAnnotationValidators(int annotations_size);
	QSet<QString> getSelectedFeatures();
	void showEDFInfoPopUp();
	void createComponents();
	void layoutComponents();
	QVBoxLayout *plotLayout;
	QGroupBox *plotGroup;
	QVBoxLayout *featureSelectionLayout;
	QGroupBox *featureSelectionGroup;
	QVBoxLayout *outputLayout;
	QPushButton *clearOutputButton;
	QGroupBox *outputGroup;
};

template <typename T>
class TextIDClass {
public:
	TextIDClass(QString id, QWidget* parent = 0) : mTextId(id), T(parent) {}
	void setId(QString newId) { mTextId = newId };
	QString id() const { return mTextId; }

private:
	QString mTextId;
};

typedef TextIDClass<QLineEdit> TextIdLineEdit;
typedef TextIDClass<QCheckBox> TextIdCheckBox;

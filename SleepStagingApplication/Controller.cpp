#include "Controller.h"

Controller::Controller(MainWindow *mainWindow, Model *model) : mMainWindow(mainWindow), mModel(model)
{
	connect(mMainWindow->openAnnotationFileButton, &QPushButton::clicked, this, &Controller::openAnnotationFile);
	connect(mMainWindow->generatePlotButton, &QPushButton::clicked, this, &Controller::generatePlot);
	connect(mMainWindow->openEDFFileButton, &QPushButton::clicked, this, &Controller::openEDFFile);
	connect(mMainWindow->extractFeaturesButton, &QPushButton::clicked, this, &Controller::extractFeatures);
	connect(mMainWindow, &MainWindow::EDFFileInfoSignal, this, &Controller::loadEpochs);
	connect(mMainWindow, &MainWindow::MLAlgorithmInfoSignal, this, &Controller::runMLAlgorithm);
	connect(this, &Controller::MLAlgorithmRunSignal, mMainWindow, &MainWindow::printAlgorithmResults);
	connect(this, &Controller::featuresExtractedSignal, mMainWindow, &MainWindow::notifyFeaturesExtracted);


}


Controller::~Controller()
{
}
void Controller::openAnnotationFile() {
	QString fileName = mMainWindow->getAnnotationFileName();
	if (fileName.isEmpty())
		return;
	
	QByteArray ba = fileName.toLatin1();
	const char *fileNameChar = ba.data();
	mModel->read_annotations_from_file(fileNameChar);
	
	mMainWindow->setAnnotationValidators(mModel->mAnnotations.size());

}

void Controller::generatePlot() {
	QVector<double> qAnnotations = QVector<double>::fromStdVector(mModel->mAnnotations);
	mMainWindow->generatePlot(qAnnotations);
}

void Controller::openEDFFile()
{
	string fileName = (mMainWindow->getEDFFileName()).toStdString();
	if (fileName.empty())
		return;
	mModel->mEDFFileName = fileName;
	mMainWindow->showEDFInfoPopUp();
}

void Controller::loadEpochs(double epochDuration, double fs)
{
	mModel->mEpochDuration = epochDuration;
	mModel->mFs = fs;
	mModel->load_epochs_from_EDF();
}

void Controller::setEpochInterval()
{
	mModel->mEpochsStart = mMainWindow->dataStartEpochLineEdit->text().toInt();
	mModel->mEpochsEnd = mMainWindow->dataEndEpochLineEdit->text().toInt();

}

void Controller::extractFeatures() {
	setEpochInterval();
	set<string> selectedFeatures;
	QSet<QString> selectedFeaturesQ = mMainWindow->getSelectedFeatures();
	for (auto i = selectedFeaturesQ.begin(); i != selectedFeaturesQ.end(); ++i) {
		selectedFeatures.insert((*i).toStdString());
	}
	mModel->extract_features(selectedFeatures);
	emit featuresExtractedSignal();

}
void Controller::runMLAlgorithm(map<string, string> algorithmInfo) {
	mModel->setMLAlgorithm(algorithmInfo);
	pair<mat, float> results = mModel->runMLAlgorithm();
	emit MLAlgorithmRunSignal(results, mModel->phaseMapping);
}


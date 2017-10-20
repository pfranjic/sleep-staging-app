#include "MLAlgorithms.h"


MLAlgorithms::MLAlgorithms()
{
	algorithms["LDA"] = [this](map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test) {
		return this->LDAFunction(algorithmInfo, data, test);
	};
	algorithms["SVM"] = [this](map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test) {
		return this->SVMFunction(algorithmInfo, data, test);
	};
	
}


MLAlgorithms::~MLAlgorithms()
{
}

pair<mat, float> MLAlgorithms::process(const string& name, map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test)
{
	return algorithms[name](algorithmInfo, data, test);
}


pair<mat, float> MLAlgorithms::LDAFunction(map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test)
{
	vector<unsigned int> output;
	vector<unsigned int> correctLabels;
	LinearClassifier<> classifier;
	LDA algorithm;

	algorithm.train(classifier, data);
	unsigned int correct = 0;
	unsigned int total = 0;
	for (auto point : test.elements()) {
		unsigned int result = classifier(point.input);
		output.push_back(result);
		correctLabels.push_back(point.label);
		if (result == point.label) {
			correct++;
		}
		total++;
	}
	return std::make_pair(calculateRecallAndPrecision(output, correctLabels), ((float)correct / total));
}
pair<mat, float> MLAlgorithms::SVMFunction(map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test)
{
	vector<unsigned int> output;
	vector<unsigned int> correctLabels;
	/*
	double gamma = 0.5;         // kernel bandwidth parameter
	GaussianRbfKernel<> kernel(gamma); // Gaussian kernel
	KernelClassifier<RealVector> kc;
	double C = 1000.0;          // regularization parameter
	bool bias = true;           // use bias/offset parameter
	*/
	GaussianRbfKernel<> kernel(stof(algorithmInfo["gamma"], 0));
	KernelClassifier<RealVector> kc;
	CSvmTrainer<RealVector> trainer(&kernel, stof(algorithmInfo["C"], 0), stof(algorithmInfo["bias"], 0));
	trainer.stoppingCondition().minAccuracy = 1e-1;
	trainer.train(kc, data);
	test.elements();
	unsigned int correct = 0;
	unsigned int total = 0;
	for (auto point : test.elements()) {
		unsigned int result = kc(point.input);
		output.push_back(result);
		correctLabels.push_back(point.label);
		if (result == point.label) {
			correct++;
		}
		total++;
	}

	return std::make_pair(calculateRecallAndPrecision(output, correctLabels), ((float)correct / total));

}
mat MLAlgorithms::calculateConfusionMatrix(vector<unsigned int> output, vector<unsigned int> correctLabels)
{
	mat confusionMatrix(6, 6, fill::zeros);
	for (int index = 0; index < correctLabels.size(); index++) {
		confusionMatrix(output[index], correctLabels[index]) = confusionMatrix(output[index], correctLabels[index]) + 1;
	}
	return confusionMatrix;
}
mat MLAlgorithms::calculateRecallAndPrecision(vector<unsigned int> output, vector<unsigned int> correctLabels)
{
	mat confusionMatrix = calculateConfusionMatrix(output, correctLabels);
	mat precisionRecall(6, 2, fill::zeros);

	for (int row = 0; row < confusionMatrix.n_rows; row++) {
		for (int col = 0; col < confusionMatrix.n_cols; col++) {
			precisionRecall(row, 0) = precisionRecall(row, 0) + confusionMatrix(row, col);
			precisionRecall(col, 1) = precisionRecall(col, 1) + confusionMatrix(row, col);
		}
	}
	for (int row = 0; row < confusionMatrix.n_rows; row++) {
		precisionRecall(row, 0) = confusionMatrix(row, row) / precisionRecall(row, 0);
		precisionRecall(row, 1) = confusionMatrix(row, row) / precisionRecall(row, 1);
	}

	return precisionRecall;

}
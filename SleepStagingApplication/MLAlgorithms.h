#pragma once
#ifndef _MAT_
#define _MAT_
#include <armadillo>
#include "mat.h"
#include "mex.h"
#include "armaMex.hpp"
#endif

#include "sigpack.h"
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <cmath>
#include <complex>
#include <fstream>
#include <fftw3.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include <mlpack/methods/naive_bayes/naive_bayes_classifier.hpp>
#include <shark/Core/Shark.h>
#include <shark/Data/Csv.h>
#include <shark/Algorithms/Trainers/LDA.h>
#include <shark/Algorithms/Trainers/CSvmTrainer.h> 
#include <shark/Models/Kernels/GaussianRbfKernel.h> 
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> 
#include <shark/Data/DataDistribution.h> 

#include <shark/Models/NearestNeighborClassifier.h>
#include <shark/Algorithms/NearestNeighbors/TreeNearestNeighbors.h>
#include <shark/Models/Trees/KDTree.h>
#include <utility>
#include <functional>
extern "C"
{
#include "stdio.h"
#include "stdlib.h"
#include "edflib.h"
}
using namespace shark;
using namespace mlpack;
using namespace mlpack::neighbor;
using namespace mlpack::metric;
using namespace arma;
using namespace sp;
using namespace std;
using namespace naive_bayes;
class MLAlgorithms
{
	map<string, function<pair<mat, float>(map<string, string>, ClassificationDataset, ClassificationDataset)>> algorithms;
	pair<mat, float> LDAFunction(map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test);
	pair<mat, float> SVMFunction(map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test);
	mat calculateRecallAndPrecision(vector<unsigned int> output, vector<unsigned int> correctLabels);
	mat calculateConfusionMatrix(vector<unsigned int> output, vector<unsigned int> correctLabels);

public:
	MLAlgorithms();
	~MLAlgorithms();
	pair<mat, float> process(const string& name, map<string, string> algorithmInfo, ClassificationDataset data, ClassificationDataset test);
};


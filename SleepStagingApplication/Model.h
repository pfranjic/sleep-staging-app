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
#include "MLAlgorithms.h"
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
extern "C"
{
#include "stdio.h"
#include "stdlib.h"
#include "edflib.h"
}

class Model
{
public:
	vector<double> mAnnotations;
	mat mEpochs;
	mat mFeatures;
	int mEpochsStart; 
	int mEpochsEnd;
	string mEDFFileName;
	double mFs;
	double mEpochDuration;
	map<string, string> mAlgorithmInfo;
	map<int, string> phaseMapping;
	Model();
	~Model();
	void loadData();
	void read_annotations_from_file(const char *file);
	void load_epochs_from_EDF();
	void extract_features(set<string> selectedFeatures);
	pair<mat, float> runMLAlgorithm();
	void setMLAlgorithm(map<string, string> algorithmInfo);
	
private:
	mat vec2mat(vec V, size_t cols);
	void calculate_band_ratios(vector<double> psdx, int fs, double &relative_delta,
		double &relative_theta, double &relative_alpha, double &relative_beta);
};


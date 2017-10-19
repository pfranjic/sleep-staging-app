#include "Model.h"
using namespace shark;
using namespace mlpack;
using namespace mlpack::neighbor; 
using namespace mlpack::metric; 
using namespace arma;
using namespace sp;
using namespace std;
using namespace naive_bayes;

Model::Model()
{
}


Model::~Model()
{
}

void Model::loadData()
{
}

void Model::load_epochs_from_EDF() {

	struct edf_hdr_struct hdr;
	if (edfopen_file_readonly(mEDFFileName.c_str(), &hdr, EDFLIB_DO_NOT_READ_ANNOTATIONS)) {
		switch (hdr.filetype)
		{
		case EDFLIB_MALLOC_ERROR: printf("\nmalloc error\n\n");
			break;
		case EDFLIB_NO_SUCH_FILE_OR_DIRECTORY: printf("\ncan not open file, no such file or directory\n\n");
			break;
		case EDFLIB_FILE_CONTAINS_FORMAT_ERRORS: printf("\nthe file is not EDF(+) or BDF(+) compliant\n"
			"(it contains format errors)\n\n");
			break;
		case EDFLIB_MAXFILES_REACHED: printf("\nto many files opened\n\n");
			break;
		case EDFLIB_FILE_READ_ERROR: printf("\na read error occurred\n\n");
			break;
		case EDFLIB_FILE_ALREADY_OPENED: printf("\nfile has already been opened\n\n");
			break;
		default: printf("\nunknown error\n\n");
			break;
		}
	}
	int hdl = hdr.handle;
	int n = mAnnotations.size()* mFs * mEpochDuration; 
	double *buf = (double *)malloc(sizeof(double)*n);

	int channel = 1;
	n = edfread_physical_samples(hdl, channel, n, buf);

	vector<double> buf_vec(buf, buf + n);

	colvec EEG(buf_vec); 
		
	mEpochs = vec2mat(EEG, mEpochDuration*mFs); // 3000 x nr epochs
}

void Model::calculate_band_ratios(vector<double> psdx, int fs, double &relative_delta,
	double &relative_theta, double &relative_alpha, double &relative_beta) {
	int psdx_size = psdx.size();
	int d1 = 0;
	int d2 = 4 * psdx_size / (fs / 2);
	int t1 = d2;
	int t2 = 8 * psdx_size / (fs / 2);
	int a1 = t2;
	int a2 = 12 * psdx_size / (fs / 2);
	int b1 = a2;
	int b2 = 30 * psdx_size / (fs / 2);
	double total_psdx = 0;
	for (int ind = d1; ind < b2; ind++) {
		total_psdx = total_psdx + psdx.at(ind);
	}
	// relative delta calculation
	double temp = 0;
	for (int ind = d1; ind < d2; ind++) {
		temp = temp + psdx.at(ind);
	}
	relative_delta = temp / total_psdx;

	temp = 0;
	for (int ind = t1; ind < t2; ind++) {
		temp = temp + psdx.at(ind);
	}
	relative_theta = temp / total_psdx;

	temp = 0;
	for (int ind = a1; ind < a2; ind++) {
		temp = temp + psdx.at(ind);
	}
	relative_alpha = temp / total_psdx;

	temp = 0;
	for (int ind = b1; ind < b2; ind++) {
		temp = temp + psdx.at(ind);
	}
	relative_beta = temp / total_psdx;
}
void Model::extract_features(set<string> selectedFeatures) {
	int N = mEpochDuration * mFs;
	int input_size = mEpochDuration * mFs;
	int output_size = (input_size / 2 + 1);
	int nrEpochs = mEpochsEnd-mEpochsStart+1; // !!!
	fftw_complex *out;
	double *in;
	int nr_features = selectedFeatures.size();
	mat features(nr_features, nrEpochs);
	fftw_plan p;
	in = (double*)fftw_malloc(sizeof(double) * input_size); // static cast
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * output_size);
	for (int ind = mEpochsStart; ind <= mEpochsEnd; ind++) {

		p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
		for (int j = 0; j < mEpochs.n_rows; j++) {
			in[j] = mEpochs(j, ind);
		}
		fftw_execute(p);
		std::vector<std::pair<double, double> > xy_pts_A;

		std::complex<double> *xthat;
		xthat = reinterpret_cast<std::complex<double> *>(out);
		vector<double> psdx;
		for (int ind = 0; ind < output_size; ind++) {
			xy_pts_A.push_back(std::make_pair(ind, abs(xthat[ind])));
			psdx.push_back(abs(xthat[ind]));
		}
		double delta, theta, alpha, beta;
		calculate_band_ratios(psdx, mFs, delta, theta, alpha, beta);
		int counter = 0;
		if (selectedFeatures.find("alpha/total") != selectedFeatures.end()) {
			features(counter, ind - mEpochsStart) = alpha;
			counter++;
		}
		if (selectedFeatures.find("beta/total") != selectedFeatures.end()) {
			features(counter, ind - mEpochsStart) = beta;
			counter++;
		}
		if (selectedFeatures.find("delta/total") != selectedFeatures.end()) {
			features(counter, ind - mEpochsStart) = delta;
			counter++;
		}
		if (selectedFeatures.find("theta/total") != selectedFeatures.end()) {
			features(counter, ind - mEpochsStart) = theta;
			counter++;
		}
	}
	fftw_destroy_plan(p);
	fftw_free(in);
	mFeatures = features;
}

void Model::read_annotations_from_file(const char *file) {
	mAnnotations.clear();
	string line;
	ifstream myfile(file);
	typedef vector< tuple<std::string, int> > annotation_tuples;
	getline(myfile, line);
	getline(myfile, line);
	getline(myfile, line);
	annotation_tuples tl;
	std::map<string, int> phase_mapping_str_to_int;
	phase_mapping_str_to_int["Sleep_stage_W"] = 6; 
	phase_mapping_str_to_int["Sleep_stage_1"] = 4;
	phase_mapping_str_to_int["Sleep_stage_2"] = 3;
	phase_mapping_str_to_int["Sleep_stage_3"] = 2;
	phase_mapping_str_to_int["Sleep_stage_4"] = 1;
	phase_mapping_str_to_int["Sleep_stage_R"] = 5;
	phase_mapping_str_to_int["unscored"] = 10;
	int counter = 0;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			istringstream iss(line);
			vector<string> tokens;
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter(tokens));
			tl.push_back(tuple<std::string, int>(tokens[7], stoi(tokens[9]) / 30));
			mAnnotations.insert(mAnnotations.end(), stoi(tokens[9]) / 30, (unsigned int)phase_mapping_str_to_int[tokens[7]]);
		}
		myfile.close();
	}

	for (map<string, int>::iterator i = phase_mapping_str_to_int.begin(); i != phase_mapping_str_to_int.end(); ++i)
		phaseMapping[i->second] = i->first;

}

void Model::setMLAlgorithm(map<string, string> algorithmInfo) {
	mAlgorithmInfo = algorithmInfo;
}

pair<mat, float> Model::runMLAlgorithm() {
	std::vector<RealVector> points;
	for (int col = 0; col < mFeatures.n_cols; col++) {
		RealVector point;
		for (int row = 0; row < mFeatures.n_rows; row++) {
			point.push_back(mFeatures(row, col));
		}
		points.push_back(point);
	}

	vector<double>::const_iterator first = mAnnotations.begin() + mEpochsStart;
	vector<double>::const_iterator last = mAnnotations.begin() + mEpochsEnd + 1;
	vector<unsigned int> newAnnotations(first, last); // check size
	transform(newAnnotations.begin(), newAnnotations.end(), newAnnotations.begin(),
		bind2nd(std::minus<int>(), 1));
	ClassificationDataset data = createLabeledDataFromRange(points, newAnnotations);
	float test_percentage = stof(mAlgorithmInfo["testPercentage"]) / 100;
	ClassificationDataset test = splitAtElement(data, static_cast<std::size_t>(test_percentage *data.numberOfElements()));

	MLAlgorithms mlAlgorithms;
	return mlAlgorithms.process(mAlgorithmInfo["name"], mAlgorithmInfo, data, test);

}

mat Model::vec2mat(vec V, size_t rows) {
	size_t cols = std::ceil(V.n_elem / double(rows));
	mat M = mat(V);
	M.reshape(rows, cols);
	return M;
}


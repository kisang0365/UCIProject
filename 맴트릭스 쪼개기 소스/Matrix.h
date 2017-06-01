#pragma once
#include "tree.h"

class Matrix{

public:
	vector<float*> data;
	int type;
	unsigned int floatSize;

public:
	Matrix::Matrix();
	Matrix::Matrix(int max[]);
	void Matrix::saveMatrix(Tree tree, cv::Mat m);
	void Matrix::saveFile(ofstream &fout);

	void Matrix::openFile(ifstream &fin);
};
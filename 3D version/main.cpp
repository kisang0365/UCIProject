#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Node.h"
#include "tree.h"
#include <opencv\highgui.h>

#define SUBNODE 4
using namespace std;


int main()
{
	ifstream inFile("a.vol", ios::binary | ios::in);

	char tag[3];
	char version_number;
	int encoding_identifier;

	unsigned int sizeX, sizeY, sizeZ;
	int channel_number;
	float bounding_box[6];

	inFile.read(tag, sizeof(tag));
	inFile.read(&version_number, sizeof(char));
	inFile.read((char*)&encoding_identifier, sizeof(int));
	inFile.read((char*)&sizeX, sizeof(int));
	inFile.read((char*)&sizeY, sizeof(int));
	inFile.read((char*)&sizeZ, sizeof(int));
	inFile.read((char*)&channel_number, sizeof(int));
	inFile.read((char*)bounding_box, sizeof(bounding_box));

	inFile.seekg(0, ios::end);
	unsigned int length = ((unsigned int)inFile.tellg() - 48) / 4;
	inFile.seekg(48, ios::beg);

	float* data;
	data = new float[length + 1];
	inFile.read((char*)data, sizeof(float)*length);

	float ***A = new float**[sizeX];

	for (unsigned int i = 0; i < sizeX; i++){
		
		A[i] = new float *[sizeY];
		for (unsigned int j = 0; j < sizeY; j++){
			A[i][j] = new float[sizeZ];
		}
	}

	for (unsigned int k = 0; k < sizeZ; k++){
		for (unsigned int i = 0; i < sizeX; i++){
			for (unsigned int j = 0; j < sizeY; j++){
				A[i][j][k] = data[(k*sizeY + j)*sizeX + i];
			}
		}
	}

	inFile.close();

	int sz[] = {sizeY, sizeX, sizeZ};
	//sizeY, sizeX를 가로 세로로 잡는다.
	cv::Mat m(3, sz, CV_8UC3, cv::Scalar(0));
	cv::Mat m1(sizeY, sizeX, CV_8UC1, cv::Scalar(0));
	cv::Mat m2 = cv::Mat::zeros(sizeY, sizeX, CV_32FC1);
	std::vector<cv::Mat> images(sizeZ);
	//32FC1 
	int sum = 0;
	for (unsigned int k = 0; k < sizeZ; k++){
		images[k] = cv::Mat(sizeY, sizeX, CV_8UC3, cv::Scalar(0));
		for (unsigned int i = 0; i < sizeY; i++){
			for (unsigned int j = 0; j < sizeX; j++){
				//cout <<A[i][j];
				/*
				if (A[j][i] == 0) m1.at<uchar>(i, j) = 0;
				else
				m1.at<uchar>(i, j) = (A[j][i]*255*2)/1;
				*/
				int v = static_cast<int>(floor(A[j][i][k] * 255.0f + 0.5f));
				if (v < 0) v = 0;
				if (v > 255) v = 255;
				unsigned char x = static_cast<uchar>(v);
				m.at<cv::Vec3b>(i, j, k) = cv::Vec3b(x, x, x);
				images[k].at<cv::Vec3b>(i, j) = cv::Vec3b(x, x, x);
			}
		}
		//string pictureName = "first";
		//pictureName += std::to_string(k);
		//pictureName += ".png";
		//cout << pictureName << endl;
		//cv::imwrite(pictureName, images[k]);
		//cout << k << endl;
	}

	/*
	//tree create
	Tree tree;
	//build start
	int start = tree.build(0, 0, 0, sizeX, sizeY, sizeZ, m);
	
	for (unsigned int k = 0; k < sizeZ; k++){
		for (unsigned int i = 0; i < sizeY; i++){
			for (unsigned int j = 0; j < sizeX; j++){
				 images[k].at<cv::Vec3b>(i, j) = m.at<cv::Vec3b>(i, j, k);
			}
		}
		string pictureName = "second";
		pictureName += std::to_string(k);
		pictureName += ".png";
		cout << pictureName << endl;
		cv::imwrite(pictureName, images[k]);
		cout << k << endl;
	}
	
	{
		ofstream fout;
		fout.open("tree.bin", ios::binary);
		tree.saveTree(fout);
	}
	*/
	std::cout << "----------------------------------" << endl;
	
	{
		ifstream fin;
		fin.open("tree.bin", ios::binary);

		Tree loadTree;
		loadTree.loadTree(fin);
		loadTree.makeB(sizeX, sizeY, sizeZ, m);

	}
	for (unsigned int i = 0; i < sizeX; i++){
		for (unsigned int j = 0; j < sizeY; j++){
			delete[] A[i][j];
		}
	}
	for (unsigned int i = 0; i < sizeX; i++){
		delete[] A[i];
	}
	delete[] A;
	
}
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


	cout << inFile.tellg() << endl;

	inFile.seekg(0, ios::end);
	unsigned int length = ((unsigned int)inFile.tellg() - 48) / 4;
	//cout << length << endl;
	//cout << "float length : " << inFile.tellg() << endl;
	//cout << "size length: " << (sizeX*sizeY*sizeZ) / 4 << endl;
	inFile.seekg(48, ios::beg);
	//cout << (float)inFile.peek() << endl;

	float* data;
	data = new float[length + 1];
	inFile.read((char*)data, sizeof(float)*length);

	//cout << tag[0] << tag[1] << tag[2] << endl;;
	//cout << version_number << encoding_identifier << endl;
	//cout << sizeX << " " << sizeY << " " << sizeZ << " " << channel_number << endl;
	//cout << bounding_box[0] << " " << bounding_box[1] << " " << bounding_box[2] << " " << bounding_box[3] << " "
	//	<< bounding_box[4] << " " << bounding_box[5] << endl;

	float **A = new float*[sizeX];
	for (unsigned int i = 0; i < sizeX; i++){
		A[i] = new float[sizeY];
	}
	int z = 29;
	for (unsigned int i = 0; i < sizeX; i++){
		for (unsigned int j = 0; j < sizeY; j++){
			A[i][j] = data[(z*sizeY + j)*sizeX + i];
			//		cout << A[i][j]<< " ";
		}
		//	cout << endl;
	}

	cout << endl;

	inFile.close();

	//sizeY, sizeX를 가로 세로로 잡는다.
	cv::Mat m1(sizeY, sizeX, CV_8UC1, cv::Scalar(0));
	cv::Mat m2 = cv::Mat::zeros(sizeY, sizeX, CV_32FC1);
	//32FC1 
	int sum = 0;
	for (unsigned int i = 0; i < sizeY; i++){
		for (unsigned int j = 0; j < sizeX; j++){
			//cout <<A[i][j];
			/*
			if (A[j][i] == 0) m1.at<uchar>(i, j) = 0;
			else
			m1.at<uchar>(i, j) = (A[j][i]*255*2)/1;
			*/
			int v = static_cast<int>(floor(A[j][i] * 255.0f + 0.5f));
			if (v < 0) v = 0;
			if (v > 255) v = 255;
			m1.at<uchar>(i, j) = static_cast<uchar>(v);

			m2.at<float>(i, j) = A[j][i];
			if (m2.at<float>(i, j) != 0) sum++;
		}
	}

	//Node node(a);
	//cout << "node : " << node.data[1] << endl;

	//Tree tree(SUBNODE);

	//cv::imwrite("b.png", m2);

	//int m1Picxel = cv::countNonZero(m1);
	//int total = sizeX*sizeY;
	//cout << "m1 : " << m1Picxel << " " << total << " " << "sum : " << sum;


	
	//cv::imshow("image", m1);
	//cv::waitKey(0);
	cv::imwrite("b.png", m1);
	//CV_8UC1 covert to CV_8UC3

	cv::Mat m3(sizeY, sizeX, CV_8UC3, cv::Scalar(0));
	for (unsigned int i = 0; i < sizeY; i++){
		for (unsigned int j = 0; j < sizeX; j++){
			unsigned char x = m1.at<uchar>(i, j);
			m3.at<cv::Vec3b>(i, j) = cv::Vec3b(x, x, x);
		}
	}

	//Node node(60, 60, a);
	//cout << "is noll" << node.BlackPicxel(60, 60) << endl;

	Tree tree;

	int start = tree.build(0, 0, sizeX, sizeY, m3);

	cv::imshow("image", m3);
	cv::waitKey(0);
	cv::imwrite("new.png", m3);
	
	{
		ofstream fout;
		fout.open("tree.bin", ios::binary);
		tree.saveTree(fout);
	}
	cout << "----------------------------------" << endl;
	{
		ifstream fin;
		fin.open("tree.bin", ios::binary);

		Tree loadTree;
		loadTree.loadTree(fin);
		loadTree.makeB(sizeX, sizeY, m1);

	}
	
}
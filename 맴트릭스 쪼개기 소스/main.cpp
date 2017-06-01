#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <string>
#include "Node.h"
#include "tree.h"
#include "Matrix.h"
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
	int sz[] = { sizeY, sizeX, sizeZ };
	//sizeY, sizeX를 가로 세로로 잡는다.
	//cv::Mat m(3, sz, CV_8UC3, cv::Scalar(0));
	cv::Mat m = cv::Mat::zeros(3, sz, CV_32FC3);
	//cv::Mat m1(sizeY, sizeX, CV_8UC1, cv::Scalar(0));
	//cv::Mat m2 = cv::Mat::zeros(sizeY, sizeX, CV_32FC1);
	//std::vector<cv::Mat> images(sizeZ);
	//std::vector<cv::Mat> image2(sizeZ);
	//32FC1 
	int sum = 0;
	for (unsigned int k = 0; k < sizeZ; k++){
		//images[k] = cv::Mat(sizeY, sizeX, CV_8UC3, cv::Scalar(0));
		//image2[k] = cv::Mat(sizeY, sizeX, CV_8UC3, cv::Scalar(0));
		for (unsigned int i = 0; i < sizeY; i++){
			for (unsigned int j = 0; j < sizeX; j++){
				//cout <<A[i][j];
				
			//	if (A[j][i] == 0) m1.at<uchar>(i, j) = 0;
			//	else
			//	m1.at<uchar>(i, j) = (A[j][i]*255*2)/1;
				float x = A[j][i][k];
				//int v = static_cast<int>(floor(A[j][i][k] * 255.0f + 0.5f));
				//if (v < 0) v = 0;
				//if (v > 255) v = 255;
				//unsigned char x = static_cast<uchar>(v);
				m.at<cv::Vec3f>(i, j, k) = cv::Vec3f(x, x, x);
				//images[k].at<cv::Vec3b>(i, j) = cv::Vec3b(x, x, x);
			}
		}
		//string pictureName = "first";
		//pictureName += std::to_string(k);
		//pictureName += ".png";
		//cout << pictureName << endl;
		//cv::imwrite(pictureName, images[k]);
		//cout << k << endl;
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

	//tree create
	Tree tree;
	//build start
	int start = tree.build(0, 0, 0, sizeX, sizeY, sizeZ, m);
	/*
	for (unsigned int k = 0; k < sizeZ; k++){
		for (unsigned int i = 0; i < sizeY; i++){
			for (unsigned int j = 0; j < sizeX; j++){
				 images[k].at<cv::Vec3b>(i, j) = m.at<cv::Vec3b>(i, j, k);
			}
		}
//		string pictureName = "first";
//		pictureName += std::to_string(k);
//		pictureName += ".png";
//		cout << pictureName << endl;
//		cv::imwrite(pictureName, images[k]);
//		cout << k << endl;
	}*/
	/*
	{
		//tree 생성
		ofstream fout;
		fout.open("tree.bin", ios::binary);
		tree.saveTree(fout);

		//matrix 생성
		Matrix matrix(tree.max);
		matrix.saveMatrix(tree, m);

		ofstream fmat;
		fmat.open("matrix.bin", ios::binary);
		matrix.saveFile(fmat);

		std::cout << "----------------------------------" << endl;
		
	}*/
	{
		std::vector<cv::Mat> image2(sizeZ);
		//32FC3 
		for (unsigned int k = 0; k < sizeZ; k++){
			image2[k] = cv::Mat(sizeY, sizeX, CV_32FC3, cv::Scalar(0));
		}
		ifstream fin;
		fin.open("tree.bin", ios::binary);

		//tree load
		Tree loadTree;
		loadTree.loadTree(fin);
		//loadTree.makeB(sizeX, sizeY, sizeZ, m);

		//matrix create
		Matrix test(loadTree.max);
		//matrix load.
		ifstream fin2;
		fin2.open("matrix.bin", ios::binary);
		test.openFile(fin2);

		//make Mat form.
		/*
		cv::Mat newM(3, sz, CV_32FC3, cv::Scalar(0));
		int n = 0;
		cout << loadTree.tree.size() << endl;
		for (unsigned int a = 0; a < loadTree.tree.size(); a++){
			//cout << a << endl;
			//if (a == 42609) cout << "42609 : " << loadTree.tree.at(a).x << " " << loadTree.tree.at(a).y << " " << loadTree.tree.at(a).z << " " << loadTree.tree.at(a).w << " " <<
			//	loadTree.tree.at(a).h << " " << loadTree.tree.at(a).l << " " << loadTree.tree.at(a).allBlack << " " << n << endl;
			if (loadTree.tree.at(a).allBlack == true){

				for (unsigned int i = 0; i < loadTree.tree.at(a).h; i++){
					for (unsigned int j = 0; j < loadTree.tree.at(a).w; j++){
						image2[loadTree.tree.at(a).z].at<cv::Vec3f>(loadTree.tree.at(a).y + i, loadTree.tree.at(a).x + j) = cv::Vec3f(0, 0, 0);
						//cout << n << endl;
					}
				}
			}

			else{
				for (unsigned int i = 0; i < loadTree.tree.at(a).h; i++){
					for (unsigned int j = 0; j < loadTree.tree.at(a).w; j++){
						float x = test.data.at(n + i)[j];
						image2[loadTree.tree.at(a).z].at<cv::Vec3f>(loadTree.tree.at(a).y + i, loadTree.tree.at(a).x + j) = cv::Vec3f(x, x, x);
					}
				}
				n += loadTree.tree.at(a).h;
				//cout << "n : "<< n << endl;
			}
		}
		*/
		cout << "여기오류없음" << endl;
		cout << test.data.size() << " " << test.floatSize << endl;
		Eigen::MatrixXf ma(test.data.size(), test.floatSize);// = Eigen::MatrixXf::Random(test.floatSize, test.data.size());
		cout << "여기도?" << endl;
		for (unsigned int i = 0; i < test.data.size(); i++){
			for (unsigned int j = 0; j < test.floatSize; j++){
				//ma(i, j) = test.data.at(i)[j];
				cout << test.data.at(i)[j];
			}
			cout << endl;
		}

		Eigen::JacobiSVD<Eigen::MatrixXf> svd(ma, Eigen::ComputeThinU | Eigen::ComputeThinV);
		cout << "Its singular values are:" << endl << svd.singularValues() << endl;
		cout << "Its left singular vectors are the columns of the thin U matrix:" << endl << svd.matrixU() << endl;
		cout << "Its right singular vectors are the columns of the thin V matrix:" << endl << svd.matrixV() << endl;
		Eigen::Vector3f rhs(1, 0, 0);
		cout << "Now consider this rhs vector:" << endl << rhs << endl;
		cout << "A least-squares solution of m*x = rhs is:" << endl << svd.solve(rhs) << endl;


		/*
		for (unsigned int k = 0; k < sizeZ; k++){
		string pictureName = "second";
		pictureName += std::to_string(k);
		pictureName += ".png";
		cout << pictureName << endl;
		cv::imwrite(pictureName, image2[k]);
		cout << k << endl;
		}
		*/
	}
	
}
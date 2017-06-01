#pragma once
#include "tree.h"

Tree::Tree(){
	max[0] = 0;
	max[1] = 0;
	max[2] = 0;
	min[0] = 9999;
	min[1] = 9999;
	min[2] = 9999;
}

int Tree::build(int x, int y, int z, int w, int h, int l, cv::Mat &m){

	Node node(x, y, z, w, h, l);
	tree.push_back(node);

	int id = tree.size() - 1;

	//x=x first value y= y=first value
	//if matrix, value reverse.
	if (!node.needToSplite(x, y, z, w, h, l, m)){
		unsigned int w1 = w / 2;
		unsigned int h1 = h / 2;
		unsigned int l1 = l / 2;

		tree.at(id).children[0] = build(x, y, z, w1, h1, l1, m);
		tree.at(id).children[1] = build(x + w1, y, z, w - w1, h1, l1, m);
		tree.at(id).children[2] = build(x, y + h1, z, w1, h - h1, l1, m);
		tree.at(id).children[3] = build(x + w1, y + h1, z, w - w1, h - h1, l1, m);
		tree.at(id).children[4] = build(x, y, z + l1, w1, h1, l - l1, m);
		tree.at(id).children[5] = build(x + w1, y, z + l1, w - w1, h1, l - l1, m);
		tree.at(id).children[6] = build(x, y + h1, z + l1, w1, h - h1, l - l1, m);
		tree.at(id).children[7] = build(x + w1, y + h1, z + l1, w - w1, h - h1, l - l1, m);
	}
	else{

		if (w < 2 || h < 2 || l < 2){
			if (w >= max[0]) max[0] = w;
			if (h >= max[1]) max[1] = h;
			if (l >= max[2]) max[2] = l;

			if (w <= min[0]) min[0] = w;
			if (h <= min[1]) min[1] = h;
			if (l <= min[2]) min[2] = l;


			if (node.ifSame(x, y, z, w, h, l, m)){
				tree.at(id).allBlack = true;
			}
		}
		else{
			tree.at(id).allBlack = true;
		}
	}
	return id;
}


void Tree::insertNode(const Node node){
	Tree::tree.push_back(node);
}

void Tree::saveTree(ofstream& fout){
	
	int treeSize = tree.size();
	std::cout << "treeSize: "<<treeSize << std::endl;
	fout.write((char*)&treeSize,sizeof(int));

	for (int i = 0; i < 3; i++)
	std::cout << "max" << i<< " : "<< max[i] << std::endl;
	fout.write((char*)max, sizeof(max));

	for (int i = 0; i < 3; i++)
		std::cout << "min" << i << " : " << min[i] << std::endl;
	fout.write((char*)min, sizeof(min));

	for (unsigned int i = 0; i < Tree::tree.size(); i++){
		saveNode(Tree::tree.at(i), fout);
	}

}

void Tree::saveNode(const Node node, ofstream& fout){

	fout.write((char*)&node, sizeof(node));

}

void Tree::loadTree(ifstream& fin){
	
	int treeSize;

	fin.read(reinterpret_cast<char *>(&treeSize), sizeof(int));
	
	std::cout << "load size : " << (int)treeSize << std::endl;
	
	fin.read(reinterpret_cast<char *>(max), sizeof(max));

	std::cout << "load max : " << (int)max[0] <<" "<< (int)max[1] << " "<<(int)max[2] << std::endl;

	fin.read(reinterpret_cast<char *>(min), sizeof(min));

	std::cout << "load max : " << (int)min[0] << " " << (int)min[1] << " " << (int)min[2] << std::endl;

	for (int i = 0; i < (int)treeSize; i++){
		Node node;
		fin.read(reinterpret_cast<char *>(&node), sizeof(node));
		tree.push_back(node);
	}
}

void Tree::makeB(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, cv::Mat m){

	std::vector<cv::Mat> tempM(sizeZ);
	std::cout << "makeB start" << std::endl;
	for (unsigned int k = 0; k < sizeZ; k++){
		tempM[k] = cv::Mat(sizeY, sizeX, CV_32FC3, cv::Scalar(0));
		for (unsigned int i = 0; i < sizeY; i++){
			for (unsigned int j = 0; j < sizeX; j++){
				tempM[k].at<cv::Vec3f>(i, j) = m.at<cv::Vec3f>(i, j, k);
			}
		}
	}

	for (unsigned int num = 0; num < tree.size(); num++){

		if (tree.at(num).children[0] == -1){
			for (unsigned int k = 0; k < tree.at(num).l; k++){
				for (unsigned int i = 0; i < tree.at(num).w; i++){
					tempM[tree.at(num).z+k].at<cv::Vec3f>(tree.at(num).y, tree.at(num).x + i) = cv::Vec3f(0, 255, 0);
					tempM[tree.at(num).z+k].at<cv::Vec3f>(tree.at(num).y + tree.at(num).h - 1, tree.at(num).x + i) = cv::Vec3f(0, 255, 0);

				}
			}
			//col
			for (unsigned int k = 0; k < tree.at(num).l; k++){
				for (unsigned int i = 0; i < tree.at(num).h; i++){
					tempM[tree.at(num).z + k].at<cv::Vec3b>(tree.at(num).y + i, tree.at(num).x) = cv::Vec3b(0, 255, 0);
					tempM[tree.at(num).z + k].at<cv::Vec3b>(tree.at(num).y + i, tree.at(num).x + tree.at(num).w - 1) = cv::Vec3b(0, 255, 0);
				}
			}
		}
	}

	for (unsigned int k = 0; k < sizeZ; k++){
		string pictureName = "makeB";
		pictureName += std::to_string(k);
		pictureName += ".png";
		cout << pictureName << endl;
		cv::imwrite(pictureName, tempM[k]);
		cout << k << endl;
	}
}
#pragma once
#include "tree.h"

Tree::Tree(){
}

int Tree::build(int x, int y, int w, int h, cv::Mat &m){
	
	Node node(x, y, w, h);
	tree.push_back(node);
	//std::cout << "node x : " << node.x << " node y : " << node.y << " node w : " << node.w << " node h : " << node.h << " children[0] : " << node.children[0] << std::endl;
	
	int id = tree.size() - 1;
	
	//x=x first value y= y=first value
	//if matrix, value reverse.
	if (!node.needToSplite(x, y, w, h, m)){
		unsigned int w1 = w / 2;
		unsigned int h1 = h / 2;
		tree.at(id).children[0] = build(x, y, w1, h1,m);
		tree.at(id).children[1] = build(x + w1, y, w - w1, h1,m);
		tree.at(id).children[2] = build(x, y + h1, w1, h - h1,m);
		tree.at(id).children[3] = build(x + w1, y + h1, w - w1, h - h1,m);
	}

	else{
		//std::cout << "---node x : " << node.x << " node y : " << node.y << " node w : " << node.w << " node h : " << node.h << " children[0] : " << node.children[0] << std::endl;

		//row
		for (int i = 0; i < w; i++){
			m.at<cv::Vec3b>(y,x+i) = cv::Vec3b(0, 255, 0);
			m.at<cv::Vec3b>(y+h-1, x+i) = cv::Vec3b(0, 255, 0);

		}
		//col
		for (int i = 0; i < h; i++){
			m.at<cv::Vec3b>(y+i, x) = cv::Vec3b(0, 255, 0);
			m.at<cv::Vec3b>(y+i, x+w-1) = cv::Vec3b(0, 255, 0);
		}
	}
	return id;
}


/*
int Tree::build(int x, int y, int z, int w, int h, int l, cv::Mat m){

Node node(x, y, z, w, h, l);
tree.push_back(node);

int id = tree.size() - 1;
if (!node.needToSplite(x, y, z, w, h, l)){
unsigned int w1 = w / 2;
unsigned int h1 = h / 2;
unsigned int l1 = l / 2;
tree.at(id).children[0] = build(x, y, z, w1, h1, l1, m);
tree.at(id).children[1] = build(x + w1, y, z, w - w1, h1, l1, m);
tree.at(id).children[2] = build(x, y + h1, z, w1, h - h1, l1, m);
tree.at(id).children[3] = build(x + w1, y + h1, z, w - w1, h - h1, l1, m);
tree.at(id).children[4] = build(x, y, z+l1, w1, h1, l-l1, m);
tree.at(id).children[5] = build(x + w1, y, z+l1, w - w1, h1, l - l1, m);
tree.at(id).children[6] = build(x, y + h1, z+l1, w1, h - h1, l - l1, m);
tree.at(id).children[7] = build(x + w1, y + h1, z+l1, w - w1, h - h1, l - l1, m);
}

else{
node.saveNode(x,y,z, w,h,l,m);
}
return id;
}
*/

void Tree::insertNode(const Node node){
	Tree::tree.push_back(node);
}

void Tree::saveTree(ofstream& fout){
	
	int treeSize = tree.size();
	std::cout << "treeSize: "<<treeSize << std::endl;
	fout.write((char*)&treeSize,sizeof(int));

	for (unsigned int i = 0; i < Tree::tree.size(); i++){
		saveNode(Tree::tree.at(i), fout);
	}

}

void Tree::saveNode(const Node node, ofstream& fout){
	

	fout.write((char*)&node, sizeof(node));
	//for (int i = 0; i < SPLITE_SIZE; i++)
		//fout << node.children[i];
	//fout << node.x<< node.y<<node.w<<node.h;
	//for (unsigned int i = 0; i < node.data.size(); i++){
		//fout << node.data.at(i);
	//}
}

void Tree::loadTree(ifstream& fin){
	
	int treeSize;

	fin.read(reinterpret_cast<char *>(&treeSize), sizeof(int));
	
	std::cout << "load size : " << (int)treeSize << std::endl;
	for (int i = 0; i < (int)treeSize; i++){
		Node node;
		fin.read(reinterpret_cast<char *>(&node), sizeof(node));
		//std::cout << "node x : " << node.x << " node y : " << node.y << " node w : " << node.w << " node h : " << node.h << " children[0] : " << node.children[0] << std::endl;
		tree.push_back(node);
	}
}

void Tree::makeB(unsigned int sizeX, unsigned int sizeY, cv::Mat m){

	cv::Mat tempM(sizeY, sizeX, CV_8UC3, cv::Scalar(0));
	for (unsigned int i = 0; i < sizeY; i++){
		for (unsigned int j = 0; j < sizeX; j++){
			unsigned char x = m.at<uchar>(i, j);
			tempM.at<cv::Vec3b>(i, j) = cv::Vec3b(x, x, x);
		}
	}

	for (unsigned int num = 0; num < tree.size(); num++){

		if (tree.at(num).children[0] == -1){
		for (unsigned int i = 0; i < tree.at(num).w; i++){
			tempM.at<cv::Vec3b>(tree.at(num).y, tree.at(num).x + i) = cv::Vec3b(0, 255, 0);
			tempM.at<cv::Vec3b>(tree.at(num).y + tree.at(num).h - 1, tree.at(num).x + i) = cv::Vec3b(0, 255, 0);

		}
		//col
		for (unsigned int i = 0; i < tree.at(num).h; i++){
			tempM.at<cv::Vec3b>(tree.at(num).y + i, tree.at(num).x) = cv::Vec3b(0, 255, 0);
			tempM.at<cv::Vec3b>(tree.at(num).y + i, tree.at(num).x + tree.at(num).w - 1) = cv::Vec3b(0, 255, 0);
		}
		}
	}
	cv::imwrite("makeB.png", tempM);
}
#pragma once
#include "Node.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Tree{
public:
	vector<Node> tree;
	int max[3];
	int min[3];

public:
	Tree();
public:
	void insertNode(const Node node);
	int Tree::build(int x, int y, int z, int w, int h, int l, cv::Mat &m);
	int Tree::rebuild(int x, int y, int w, int h);
	void Tree::spliteNode(Node node);

	void Tree::saveTree(ofstream& fout);
	void Tree::saveNode(const Node node, ofstream& fout);
	void Tree::loadTree(ifstream& fin);
	void Tree::makeB(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, cv::Mat m);
	
};
#pragma once
#include <iostream>
#include <opencv\highgui.h>
#include <cstddef>
#include <vector>

#define SPLITE_SIZE 4
//#define SPLITE_SIZE 8
class Node{
public:
	int children[4];
	//int children[8];
	unsigned int x, y, w, h;
	//unsigned int z,l;

public:
	Node::Node();
	Node::Node(unsigned int tempX, unsigned int tempY, unsigned int tempW, unsigned int tempH);
	void Node::saveNode(int x, int y, int w, int h);
	bool Node::needToSplite(unsigned int x, unsigned int y, unsigned int w, unsigned int h, cv::Mat m);
	bool Node::ifSame(unsigned int x, unsigned int y, unsigned int w, unsigned int h, cv::Mat m);

};
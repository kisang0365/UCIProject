#pragma once
#include <iostream>
#include <opencv\highgui.h>
#include <cstddef>
#include <vector>

#define SPLITE_SIZE 8
class Node{
public:
	int children[8];
	unsigned int x, y, w, h, z, l;

public:
	Node::Node();
	Node::Node(unsigned int tempX, unsigned int tempY, unsigned int tempZ, unsigned int tempW, unsigned int tempH, unsigned int tempL);
	bool Node::needToSplite(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l, cv::Mat m);
	bool Node::ifSame(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l, cv::Mat m);
};
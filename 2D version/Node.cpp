#pragma once
#include "Node.h"

Node::Node(){

}
/*
Node::Node(unsigned int tempX, unsigned int tempY, unsigned int tempZ, unsigned int tempW, unsigned int tempH, unsigned int tempL)
{
	x = tempX;
	y = tempY;
	z = tempZ;
	w = tempW;
	h = tempH;
	l = tempL;
	for (int i = 0; i < SPLITE_SIZE; i++){
		children[i] = 0;
	}

}
*/
Node::Node(unsigned int tempX, unsigned int tempY, unsigned int tempW, unsigned int tempH)
{
	x = tempX;
	y = tempY;
	w = tempW;
	h = tempH;
	for (int i = 0; i < SPLITE_SIZE; i++){
		children[i] = -1;
	}

}

bool Node::needToSplite(unsigned int x, unsigned int y, unsigned int w, unsigned int h, cv::Mat m){

	if (w < 10 || h < 10) return true;
	if (ifSame(x, y, w, h, m) == true)
		return true;
	
	return false;
}
/*
bool Node::needToSplite(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l){

	if (w < 4 || h < 4 || l < 4) return true;
	//if (BlackPicxel(sizeX, sizeY) == true)
	//return true;

	return false;
}
*/

void Node::saveNode(int x, int y, int w, int h){
	std::cout << std::endl;
}

bool Node::ifSame(unsigned int x, unsigned int y, unsigned int w, unsigned int h, cv::Mat m){

	for (unsigned int i = 0; i < h; i++){
		for (unsigned int j = 0; j < w; j++){
			if (m.at<cv::Vec3b>(y + i, x + j) != m.at<cv::Vec3b>(y, x))
				return false;
		}
	}
	return true;
}

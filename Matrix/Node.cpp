#pragma once
#include "Node.h"

Node::Node(){

}

Node::Node(unsigned int tempX, unsigned int tempY, unsigned int tempZ, unsigned int tempW, unsigned int tempH, unsigned int tempL)
{
	x = tempX;
	y = tempY;
	z = tempZ;
	w = tempW;
	h = tempH;
	l = tempL;
	allBlack = false;
	for (int i = 0; i < SPLITE_SIZE; i++){
		children[i] = -1;
	}
}

bool Node::needToSplite(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l, cv::Mat m){

if (w < 2 || h < 2 || l < 2) return true;
if (ifSame(x, y, z, w, h, l, m) == true)
return true;

return false;
}

bool Node::needToImageSplite(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l, cv::Mat m){

	if (w < 2 || h < 2 || l < 2) return true;
	if (ifSame(x, y, z, w, h, l, m) == true)
		return true;

	return false;
}

bool Node::ifSame(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l, cv::Mat m){

	for (unsigned int k = 0; k < l; k++){
		for (unsigned int i = 0; i < h; i++){
			for (unsigned int j = 0; j < w; j++){
				if (m.at<cv::Vec3b>(y + i, x + j, z+k) != m.at<cv::Vec3b>(y, x, z))
					return false;
			}
		}
	}
	return true;
}

bool Node::ifImageSame(unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int l, cv::Mat m){
	/*
	for (unsigned int i = 0; i < h; i++){
			for (unsigned int j = 0; j < w; j++){
				if (m.at<cv::Vec3b>(y + i, x + j, z + k) != m.at<cv::Vec3b>(y, x, z))
					return false;
			}
		}*/
	return true;
}
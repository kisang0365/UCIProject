#pragma once
#include <iostream>
#include <opencv\highgui.h>
#include <cstddef>
#include <vector>

class Data{
public:
	float* d;

public:
	Data(int size){
		d = new float[size];
	}

};
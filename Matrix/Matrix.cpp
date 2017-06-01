#pragma once
#include "Matrix.h"

Matrix::Matrix(){
	type = 0;
	floatSize = 0;
}

Matrix::Matrix(int max[])
{
	if (max[2] == 1){
		if (max[1] >= max[0]) type = 1;   //h->1 splite, w fix.
		else type = 2; //w->1 splite, h fix.
	}
	else if (max[1] == 1){
		if (max[2] >= max[0]) type = 3;   //l->1 splite, w fix.
		else type = 4; //w->1 splite, l fix.
	}
	else{
		if (max[2] >= max[1]) type = 5; //l->1 splite, h fix
		else type = 6; // h->1 splite, l fix.
	}
	if (type == 1 || type == 3) floatSize = max[0];
	else if (type == 2 || type == 5) floatSize = max[1];
	else floatSize = max[2];

	cout << "floatSize " << floatSize <<" " << type<< endl;
}

void Matrix::saveMatrix(Tree tree, cv::Mat m){
	cout << "treeSize : " << tree.tree.size() << "type "<< type <<endl;
	for (unsigned int i = 0; i < tree.tree.size(); i++){
	//	cout << i << " ";
		if (tree.tree.at(i).allBlack == false){
			if (type == 1){
				//cout << tree.tree.at(i).x << " " << tree.tree.at(i).y << " " << tree.tree.at(i).z << " " << tree.tree.at(i).w << endl;
				//cout << "allBlack : " << tree.tree.at(i).allBlack << endl;
			//	cout << tree.tree.at(i).x << " " << tree.tree.at(i).y << " " << tree.tree.at(i).z << " " << tree.tree.at(i).w << " " << tree.tree.at(i).h << " " << tree.tree.at(i).l<<" "<<endl;
				for (unsigned int j = 0; j < tree.tree.at(i).h; j++){
					data.push_back(new unsigned char[floatSize]);
					int id = data.size() - 1;
					for (unsigned int k = 0; k < floatSize; k++){
						if (k < tree.tree.at(i).w)
							//cout << (int)m.at<cv::Vec3b>(tree.tree.at(i).x + k, tree.tree.at(i).y + j, tree.tree.at(i).z)[0] << " " << endl;
							data.at(id)[k] = m.at<cv::Vec3b>(tree.tree.at(i).y + j, tree.tree.at(i).x + k, tree.tree.at(i).z)[0]; //여기다가.
						else
							data.at(id)[k] = 0;
					}
				} //cout << endl;
			}
			else if (type == 2){
				
				for (unsigned int j = 0; j < tree.tree.at(i).w; j++){
					data.push_back(new unsigned char[floatSize]);
					int id = data.size() - 1;
					for (unsigned int k = 0; k < floatSize; k++){
						if (k < tree.tree.at(i).h)
							data.at(id)[k] = m.at<cv::Vec3b>(tree.tree.at(i).x + j, tree.tree.at(i).y + k, tree.tree.at(i).z)[0];
						else
							data.at(id)[k] = 0;
					}
				}
			}
			else if (type == 3){

				for (unsigned int j = 0; j < tree.tree.at(i).l; j++){
					data.push_back(new unsigned char[floatSize]);
					int id = data.size() - 1;
					for (unsigned int k = 0; k < floatSize; k++){
						if (k < tree.tree.at(i).w)
							data.at(id)[k] = m.at<cv::Vec3b>(tree.tree.at(i).x + k, tree.tree.at(i).y, tree.tree.at(i).z+j)[0];
						else
							data.at(id)[k] = 0;
					}
				}
			}

			else if (type == 4){

				for (unsigned int j = 0; j < tree.tree.at(i).w; j++){
					data.push_back(new unsigned char[floatSize]);
					int id = data.size() - 1;
					for (unsigned int k = 0; k < floatSize; k++){
						if (k < tree.tree.at(i).l)
							data.at(id)[k] = m.at<cv::Vec3b>(tree.tree.at(i).x + j, tree.tree.at(i).y, tree.tree.at(i).z + k)[0];
						else
							data.at(id)[k] = 0;
					}
				}
			}
			else if (type == 5){

				for (unsigned int j = 0; j < tree.tree.at(i).l; j++){
					data.push_back(new unsigned char[floatSize]);
					int id = data.size() - 1;
					for (unsigned int k = 0; k < floatSize; k++){
						if (k < tree.tree.at(i).h)
							data.at(id)[k] = m.at<cv::Vec3b>(tree.tree.at(i).x , tree.tree.at(i).y + k, tree.tree.at(i).z + j)[0];
						else
							data.at(id)[k] = 0;
					}
				}
			}

			else {

				for (unsigned int j = 0; j < tree.tree.at(i).h; j++){
					data.push_back(new unsigned char[floatSize]);
					int id = data.size() - 1;
					for (unsigned int k = 0; k < floatSize; k++){
						if (k < tree.tree.at(i).l)
							data.at(id)[k] = m.at<cv::Vec3b>(tree.tree.at(i).x, tree.tree.at(i).y + j, tree.tree.at(i).z + k)[0];
						else
							data.at(id)[k] = 0;
					}
				}
			}
		}
	}
	std::cout << "----------------------------------" << endl;
}

void Matrix::saveFile(ofstream &fout){
	int dataSize = data.size();
	std::cout << "data size : " << data.size() <<endl;
	fout.write((char*)&dataSize, sizeof(int));
	std::cout << "okay" << endl;
	for (unsigned int i = 0; i < data.size(); i++){
		fout.write((char*)data.at(i), sizeof(unsigned char) * floatSize);
	}
}

void Matrix::openFile(ifstream &fin){

	int dataSize;

	fin.read(reinterpret_cast<char *>(&dataSize), sizeof(int));

	std::cout <<"data size : "<< dataSize << std::endl;
	
	for (int i = 0; i < (int)dataSize; i++){
		//std::cout << i << endl;
		unsigned char* d;
		d = new unsigned char[floatSize];
		fin.read(reinterpret_cast<char *>(d), sizeof(unsigned char) * floatSize);
		data.push_back(d);
	}

}
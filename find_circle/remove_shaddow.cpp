// remove_shaddow.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include <math.h>
#include<fstream>
#include <iostream>

using namespace std;
using namespace cv;

Mat ShadowExtraction(Mat src, int type = 1);
vector<float> loadtxt(string loadPath);
vector<float> loadCsv(string loadPath);
void Conver2cloud(Mat src);
int _tmain(int argc, _TCHAR* argv[])
{
	char filename[] = "D:/code_project/3D/img.png";
	Mat img = imread(filename);
	//resize(img, img, Size(720, (720 * img.rows / img.cols)));//将图像的尺寸缩放到适合屏幕观看
	imshow("Previous", img);
	Mat img_after_shaddow = ShadowExtraction(img);
	//Conver2cloud(img_after_shaddow);

	
	waitKey(0);
	return 0;
}
void Conver2cloud(Mat img){
	vector<float> src = loadtxt("point_cloud_ori.xyz");
	Mat z = Mat(1, 5044624, CV_32FC1);
	int n = 0;
	for (int i = 0; i < src.size(); i = i + 3){
		/*x.at<float>(0, n) = src[i];
		y.at<float>(0, n) = src[i + 1];*/
		z.at<float>(0, n) = src[i + 2];
		n++;
	}
	ofstream point_cloud, point_cloud_shaddow,point_cloud_fix_z;
	point_cloud.open("point_cloud_process.xyz");
	point_cloud_shaddow.open("point_cloud_shaddow.xyz");
	//point_cloud_fix_z.open("point_cloud_fix_z.xyz");

	//vector<float> std = loadCsv("Z_map.csv");
	Mat Z_map = Mat(2054, 2456, CV_32FC1);
	int X = 0, Y = 0;
	for (int num = 0; num < 5044624; num++){

		/*if (STD.at<float>(Y, X)>5){
		Z_map.at<float>(Y, X) = z.at<float>(0, num);
		}
		else {
		Z_map.at<float>(Y, X) = 0;
		}*/

		Z_map.at<float>(Y, X) = -1*z.at<float>(0, num);

		X++;
		if (X == 2456){
			X = 0;
			Y++;
		}
	}


	std::vector<cv::Mat> rgbChannels(3);
	split(img, rgbChannels);
	//point_cloud << rgbChannels[0].at<uchar>(0, 0) << endl;

	//for (int y = 0; y < Z_map.rows; y++){
	//for (int x = 0; x < Z_map.cols; x++){
	////point_cloud << (int)rgbChannels[0].at<uchar>(y, x) << " " << (int)rgbChannels[1].at<uchar>(y, x) << " " << (int)rgbChannels[2].at<uchar>(y, x) << endl;
	//	point_cloud_fix_z << x << " " << y << " " << Z_map.at<float>(y, x) << endl;
	//}
	//}

	for (int y = 0; y < Z_map.rows; y++){
		for (int x = 0; x < Z_map.cols; x++){


			//if ((img.at<Vec3b>(y, x)[0] == 255) && (img.at<Vec3b>(y, x)[1] == 0) && (img.at<Vec3b>(y, x)[2] == 0)){
			if (((int)rgbChannels[0].at<uchar>(y, x) == 255) && ((int)rgbChannels[1].at<uchar>(y, x) == 0) && ((int)rgbChannels[2].at<uchar>(y, x) == 0)){
				//point_cloud << '1' << " " << '2' << " " << '3' << endl;
				point_cloud_shaddow << x << " " << y << " " << Z_map.at<float>(y, x) << endl;
			}
			else{
				point_cloud << x << " " << y << " " << Z_map.at<float>(y, x) << endl;
			}

			// Split the image into different channels

			//point_cloud << x << " " << y << " " << Z_map.at<float>(y, x) << rgbChannels[0](y, x) << endl;
		}
	}




	point_cloud.close();
	point_cloud_shaddow.close();
	//point_cloud_fix_z.close();
}
vector<float> loadCsv(string loadPath)
{
	ifstream inFile(loadPath, ios::in);
	if (!inFile)
	{
		cout << "開啟檔案失敗！" << endl;
		exit(1);
	}
	string line;
	vector<float> a;

	while (getline(inFile, line))
	{
		//cout << "org=" << line << endl;
		//========================


		istringstream delim(line);
		string token;
		int c = 0;
		while (getline(delim, token, ','))
		{
			a.push_back(stof(token));
			c++;
		}

		//cout << "size=" << a.size() << endl;
		//for (int ii = 0; ii < a.size(); ii++)
		//{
		// cout << a[ii] << ",";
		//}
		//cout << endl;
		//========================
	}
	return a;
}
vector<float> loadtxt(string loadPath)
{
	ifstream inFile(loadPath, ios::in);
	if (!inFile)
	{
		cout << "開啟檔案失敗！" << endl;
		exit(1);
	}
	string line;
	vector<float> a;

	while (getline(inFile, line))
	{
		//cout << "org=" << line << endl;
		//========================


		istringstream delim(line);
		string token;
		int c = 0;
		while (getline(delim, token, ' '))
		{
			a.push_back(stof(token));
			c++;
		}

		//cout << "size=" << a.size() << endl;
		//for (int ii = 0; ii < a.size(); ii++)
		//{
		// cout << a[ii] << ",";
		//}
		//cout << endl;
		//========================
	}
	return a;
}

double varOf3(double x1, double x2, double x3){
	double mean = (x1 + x2 + x3) / 3;
	return (pow((x1 - mean), 2) + pow((x2 - mean), 2) + pow((x3 - mean), 2));
}
Mat ShadowExtraction(Mat src, int type) {
	int rows = src.rows, cols = src.cols;
	Mat M(rows, cols, CV_8UC1);
	double* var = new double[rows*cols];
	int i = 0;
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			if (((double)src.at<Vec3b>(y, x)[0] + (double)src.at<Vec3b>(y, x)[1] + (double)src.at<Vec3b>(y, x)[2]) > 450) {//限制亮度
				//var[i] = sqrt(varOf3((double)src.at<Vec3b>(y, x)[0], (double)src.at<Vec3b>(y, x)[1], (double)src.at<Vec3b>(y, x)[2]));
				var[i] = 0;
			}
			else {
				var[i] = 255;
			}
			i++;
		}
	}
	int j = 0;
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			M.at<uchar>(y, x) = (uchar)(var[j]);//把方差作为亮度进行赋值
			j++;
		}
	}
	imshow("apply_variance", M);
	//imshow("Before Binarization", M);
	switch (type) {
	case 1://轻度阴影提取
		//equalizeHist(M, M);//可以过滤掉颜色比较浅的部分
		break;
	case 2://强力阴影提取
		break;
	}
	imshow("after_equalizeHist", M);
	/*int block_size = 25;
	int const_value = 10;
	adaptiveThreshold(M, M, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, block_size, const_value);*/
	threshold(M, M, 70, 255, THRESH_BINARY_INV);//把图像二值化
	//bitwise_not(M,M);
	imshow("after_threshold", M);

	//medianBlur(M, M, 3);//中值滤波，去除小斑点
	imshow("Shadow", M);
	Mat M3 = src.clone();//M3是最后阴影提取后的结果
	double count = 0.0;

	for (int x = 0; x < cols; x++) {//给识别出来的阴影上色
		for (int y = 0; y < rows; y++) {
			if (M.at<uchar>(y, x) == 255) {
				M3.at<Vec3b>(y, x)[0] = 255;
				M3.at<Vec3b>(y, x)[1] = 0;
				M3.at<Vec3b>(y, x)[2] = 0;
				count++;
			}
		}
	}

	char str1[64] = "Shadow in Previous";
	char str2[64];
	sprintf_s(str2, "  ||  Weight of Shadow:%.2lf%%", 100.0*count / rows / cols);//阴影量
	strcat_s(str1, str2);
	//erode(M3, M3, Mat());//腐蚀
	//dilate(M3, M3, Mat());//膨胀
	imshow(str1, M3);//显示最后处理结果的


	//waitKey(0);

	delete[] var;
	return M3;
}
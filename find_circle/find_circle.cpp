// find_circle.cpp : 定義主控台應用程式的進入點。
//
#define PI 3.1415926
#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <math.h>
#include<fstream>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <cmath>
//#include <python.h>
//#include "matplotlibcpp.h"
using namespace cv;
using namespace std;
//namespace plt = matplotlibcpp;
int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("img.png", IMREAD_GRAYSCALE);

	Mat ori = img.clone();
	////if (img.empty()) return -1;
	//namedWindow("HelloCV", WINDOW_AUTOSIZE);
	//imshow("Origin", img);
	/*Py_Initialize();  
	PyRun_SimpleString("print 'Hello World!!'");
	Py_Finalize(); */
	

	/*SimpleBlobDetector::Params params;
	vector<KeyPoint> keypoints;
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	detector->detect(img, keypoints);*/

	threshold(img, img, 85, 255, CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(25, 25));
	//Mat element = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
	morphologyEx(img, img, MORPH_OPEN, element);

	SimpleBlobDetector::Params params = cv::SimpleBlobDetector::Params();
	params.minThreshold = 10;
	params.maxThreshold = 200;
	//params.thresholdStep = 50;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;
	//params.maxInertiaRatio = 1;
	params.filterByConvexity = true;
	params.minConvexity = 0.4;
	/*params.filterByArea = true;
	params.minArea = 1000;*/
	params.filterByCircularity = true;
	params.minCircularity = 0.1;
	params.blobColor = 255;

	SimpleBlobDetector detector(params);
	vector<KeyPoint> keypoints;
	detector.detect(img, keypoints);

	int shift_value = 20;
	int blob_radius = 85;

	for (int i = 0; i < keypoints.size(); i++){
		
			keypoints[i].pt.x += shift_value;
			keypoints[i].size = blob_radius;
			
	}


	int point_index = 108;
	int left = keypoints[point_index].pt.x - blob_radius / 2;
	int right = keypoints[point_index].pt.x + blob_radius / 2;

	
	fstream fout;
	// opens an existing csv file or creates a new file.
	/*fout.open("test_circle_108_angle.csv", ios::out | ios::app);
	for (int i = left; i < right; i++){		
		fout << (int)ori.at<uchar>(keypoints[point_index].pt.y,i) << ",";
		ori.at<uchar>(keypoints[point_index].pt.y, i) = 255;
	}*/

	int y;
	int x;
	int offset = 0;
	int angle = 150;
	string filename = "test_circle_108_angle_" + to_string(angle)+".csv";
	fout.open(filename, ios::out | ios::app);
	for (int r = -blob_radius/2  + offset; r < blob_radius/2 - offset; r++){

		y = (int)keypoints[point_index].pt.y + r*cos(angle * 3.14 / 180.0f);
		x = (int)keypoints[point_index].pt.x + r*sin(angle * 3.14 / 180.0f);

		/*y = (int)keypoints[point_index].pt.y - r;
		x = (int)keypoints[point_index].pt.x - r;*/

		fout << (int)ori.at<uchar>(y, x) << ",";

		//ori.at<uchar>(y, x) = 255;
	}
	/*fout << "/n";*/

	//draw line
	/*int thickness = 2;
	int LINE_8 = 8;	
	Point p1(keypoints[point_index].pt.x - blob_radius/2, keypoints[point_index].pt.y);
	Point p2(keypoints[point_index].pt.x + blob_radius/2, keypoints[point_index].pt.y);
	line(ori, p1, p2, Scalar(255, 0, 0), thickness, LINE_8);*/
	
	

	

	//draw circle in the map
	drawKeypoints(ori, keypoints, img, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("檢測結果", img); waitKey(0);
	imwrite("output.jpg", ori);

	waitKey(0);
	destroyWindow("HelloCV");
	fout.close();
	return 0;
}


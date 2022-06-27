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

using namespace cv;
using namespace std;
MatND plot_hist(Mat src);

int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("0um/img1-1.bmp",2);
	//Mat img = imread("five_step/img4-1.bmp", 2);
	/*Mat img = imread("lena.jpg");*/
	Mat binary = img.clone();
	cout << binary.dims;

	for (int row = 0; row < binary.size().height; row++){
		for (int col = 0; col < binary.size().width; col++){
			if ((int)binary.at<uchar>(row, col)>80){
				binary.at<uchar>(row, col) = 255;
			}
			else{
				binary.at<uchar>(row, col) = 0;
			}
		}
	}

	//Mat histogramImage = plot_hist(img);

	imwrite("0um/img1_binary.bmp", binary);
	//canny
	Mat canny;
	Canny(binary, canny, 100, 100);


	double scale = 0.25;
	resize(binary, binary, Size(img.cols*scale,img.rows*scale));
	resize(img, img, Size(img.cols*scale, img.rows*scale));
	resize(canny, canny, Size(canny.cols*scale, canny.rows*scale));
	//resize(histogramImage, histogramImage, Size(histogramImage.cols*0.5, histogramImage.rows*0.5));

	
	



	if (img.empty()) return -1;
	namedWindow("HelloCV", WINDOW_AUTOSIZE);
	imshow("HelloCV", img);
	imshow("binary", binary);
	imshow("canny", canny);
	//imshow("histimage", histogramImage);
	waitKey(0);
	destroyWindow("HelloCV");



	/*system("PAUSE");*/
	return 0;

}

MatND plot_hist(Mat src){
	//Initialize the histogram params
	int histogramSize = 255;
	float range[] = { 0, 255 };
	const float *ranges[] = { range };

	//parameters for the histogram image
	int histogramImageWidth = histogramSize * 4;
	int histogramImageHeight = histogramSize * 4;
	int bin_w = 4;

	//calculate histogram. Since the values in Hue channel range from 0 to 179, so we keep number of bin as 180.
	//First argument of calcHist function in channel[0] which is Hue channel, second argument is number of images, 3rd is the
	// channel number of the image which is 0 here as we are just passing single channel, 4th argument is mask which we will send as
	//empty as we want the whole mattrix, 5th argument is the output histogram, 6th argument is dimensions of the histogram which is 1 
	//here. 7th argument is number of bins and 8th argument is range of histogram
	MatND histogram;
	calcHist(&src, 1, 0, Mat(), histogram, 1, &histogramSize, ranges, true, false);
	/*cout << histogram << endl;*/

	//normalize the histogram to make sure the histogram fits in the plot
	normalize(histogram, histogram, 0, histogramSize * 3, NORM_MINMAX, -1, Mat());

	//construct the histogram as an image
	Mat histogramImage(histogramImageHeight, histogramImageWidth, CV_8UC3, Scalar(255, 255, 255));
	cout << histogram << endl;

	//draw x-axis
	line(histogramImage, Point(0, histogramImageHeight - 30), Point(histogramImageWidth, histogramImageHeight - 30), Scalar(0, 0, 0), 2, 8, 0);

	//draw origin
	line(histogramImage, Point(0, histogramImageHeight - 20), Point(0, histogramImageHeight - 40), Scalar(0, 0, 0), 2, 8, 0);
	putText(histogramImage, "0", Point(0, histogramImageHeight - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);

	//draw histogram. We go over each value of the histogram and draw a line between current and previous points. We also put the x-axis 
	// value after every 20 values.
	for (int i = 1; i < histogramSize; i++){
		line(histogramImage, Point((i - 1)*bin_w, histogramImageHeight - 30 - histogram.at<float>(i - 1)),
			Point(i*bin_w, histogramImageHeight - 30 - histogram.at<float>(i)), Scalar(0, 0, 255), 2, 8, 0);

		// draw the x axis values
		if (i % 20 == 0){
			string text = to_string(i);
			line(histogramImage, Point(i*bin_w, histogramImageHeight - 20), Point(i*bin_w, histogramImageHeight - 40), Scalar(0, 0, 0), 2, 8, 0);
			putText(histogramImage, text, Point(i*bin_w, histogramImageHeight - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);
		}
	}

	return histogramImage;
}
#include "stdafx.h"
#include <opencv2/opencv.hpp>  
using namespace cv;
int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("lena.jpg", -1);
	if (img.empty()) return -1;
	namedWindow("HelloCV", WINDOW_AUTOSIZE);
	imshow("HelloCV", img);
	waitKey(0);
	destroyWindow("HelloCV");
	return 0;

}
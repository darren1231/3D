#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<algorithm>

using namespace cv;
using namespace std;

string formatDobleValue(double val, int fixed);

int _tmain(int argc, _TCHAR* argv[])
{
	/*Mat img1 = imread("0um/img1-1.bmp", 2);
	Mat img2 = imread("0um/img1-2.bmp", 2);
	Mat img3 = imread("0um/img1-3.bmp", 2);
	Mat img4 = imread("0um/img1-4.bmp", 2);
	Mat img5 = imread("0um/img1-5.bmp", 2);*/

	string filename;
	Mat source_array[5];
	for (int index = 0; index < 5; index++){
		filename = "img2-" + to_string(index+1) + ".bmp";
		source_array[index] = imread(filename,2);
	}

	vector <Mat> source_mat = {};
	vector <Mat> binary_mat = {};
	/*Mat img;
	Mat binary_img;	
	for (int index = 1; index <= 5; index++){
		filename = "img2-" + to_string(index) + ".bmp";
		img = imread(filename);
		source_mat.push_back(img);
	}
	for (int index = 0; index < 5; index++){
		binary_img = source_mat[index].clone();
		binary_mat.push_back(binary_img);
	}*/

	Mat img1 = imread("img2-1.bmp", 2);
	Mat img2 = imread("img2-2.bmp", 2);
	Mat img3 = imread("img2-3.bmp", 2);
	Mat img4 = imread("img2-4.bmp", 2);
	Mat img5 = imread("img2-5.bmp", 2);

	Mat binary0 = img1.clone();
	Mat binary1 = img2.clone();
	Mat binary2 = img3.clone();
	Mat binary3 = img4.clone();
	Mat binary4 = img5.clone();

	int minValue;
	int maxValue;
	int gray;
	double threshod;
	double set_thresshold = 0.65;

	for (int x = 0; x < source_array[0].cols; x++){
		for (int y = 0; y < 100; y++){

			vector<int> v = {};
			for (int index = 0; index < 5; index++){
				gray = (int)(source_array[index].at<uchar>(y, x));
				v.push_back(gray);
				/*v.push_back((int)(source_mat[index].at<uchar>(y, x)));*/

			}
			/*v.push_back((int)img1.at<uchar>(y, x));
			v.push_back((int)img2.at<uchar>(y, x));
			v.push_back((int)img3.at<uchar>(y, x));
			v.push_back((int)img4.at<uchar>(y, x));
			v.push_back((int)img5.at<uchar>(y, x));*/

			maxValue = *max_element(v.begin(), v.end());
			minValue = *min_element(v.begin(), v.end());

			///////////////////////////////////////0
			if (maxValue - minValue != 0)
				threshod = (double)(v[0] - minValue) / (maxValue - minValue);
			else
				threshod = 0;
			
			if (threshod<set_thresshold)
				binary0.at<uchar>(y, x) = 0;
			else
				binary0.at<uchar>(y, x) = 255;
			////////////////////////////////////////1
			if (maxValue - minValue != 0)
				threshod = (double)(v[1] - minValue) / (maxValue - minValue);
			else
				threshod = 0;
			
			if (threshod<set_thresshold)
				binary1.at<uchar>(y, x) = 0;
			else
				binary1.at<uchar>(y, x) = 255;
			////////////////////////////////////////2
			if (maxValue - minValue != 0)
				threshod = (double)(v[2] - minValue) / (maxValue - minValue);
			else
				threshod = 0;

			if (threshod<set_thresshold)
				binary2.at<uchar>(y, x) = 0;
			else
				binary2.at<uchar>(y, x) = 255;
			////////////////////////////////////////3
			if (maxValue - minValue != 0)
				threshod = (double)(v[3] - minValue) / (maxValue - minValue);
			else
				threshod = 0;

			if (threshod<set_thresshold)
				binary3.at<uchar>(y, x) = 0;
			else
				binary3.at<uchar>(y, x) = 255;
			////////////////////////////////////////4
			if (maxValue - minValue != 0)
				threshod = (double)(v[4] - minValue) / (maxValue - minValue);
			else
				threshod = 0;

			if (threshod<set_thresshold)
				binary4.at<uchar>(y, x) = 0;
			else
				binary4.at<uchar>(y, x) = 255;

			/*if (maxValue - minValue != 0)
				threshod = (double)(v[1] - minValue) / (maxValue - minValue);
			else
				threshod = 0;
			binary2.at<uchar>(y, x) = (int)(threshod * 255);*/

			/*for (int index = 0; index < 5; index++){
				if (maxValue - minValue != 0)
					threshod = (double)(v[index] - minValue) / (maxValue - minValue);
				else
					threshod = 0;
				binary_mat[index].at<uchar>(y, x) = (int)(threshod * 255);
			}*/
				


			
			/*if (threshod!=0)
				cout << x << " " << y <<" "<<threshod<< endl;*/
		}
	}
	/*for (int index = 0; index < 5; index++)
		imwrite("binary_ntu_" + to_string(index) + ".jpg", binary_mat[index]);*/
	
	
	imwrite("binary_ntu_" + formatDobleValue(set_thresshold,2) + "_" + to_string(0) + ".jpg", binary0);	
	imwrite("binary_ntu_" + formatDobleValue(set_thresshold, 2) + "_" + to_string(1) + ".jpg", binary1);
	imwrite("binary_ntu_" + formatDobleValue(set_thresshold, 2) + "_" + to_string(2) + ".jpg", binary2);
	imwrite("binary_ntu_" + formatDobleValue(set_thresshold, 2) + "_" + to_string(3) + ".jpg", binary3);
	imwrite("binary_ntu_" + formatDobleValue(set_thresshold, 2) + "_" + to_string(4) + ".jpg", binary4);

	///*double scale = 0.25;
	//resize(binary1, binary1, Size(img1.cols*scale, img1.rows*scale));WW
	//resize(img1, img1, Size(img1.cols*scale, img1.rows*scale));*/
	//

	namedWindow("HelloCV", WINDOW_AUTOSIZE);
	imshow("HelloCV", img1);
	imshow("binary0", binary0);
	//
	waitKey(0);
	destroyWindow("HelloCV");
	return 0;

}

string formatDobleValue(double val, int fixed) {
	auto str = std::to_string(val);
	return str.substr(0, str.find(".") + fixed + 1);
}
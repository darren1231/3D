#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<algorithm>

using namespace cv;
using namespace std;

string formatDobleValue(double val, int fixed);

int _tmain(int argc, _TCHAR* argv[])
{
	

	string filename;
	Mat source_map[5];
	Mat binary_map[5];
	for (int index = 0; index < 5; index++){
		filename = "img2-" + to_string(index+1) + ".bmp";
		source_map[index] = imread(filename, 2);
		binary_map[index] = source_map[index].clone();
	}	

	int minValue;
	int maxValue;
	int gray;
	double threshod;
	double set_thresshold = 0.65;

	for (int x = 0; x < source_map[0].cols; x++){
		for (int y = 0; y < source_map[0].rows; y++){

			vector<int> v = {};
			for (int index = 0; index < 5; index++){
				gray = (int)(source_map[index].at<uchar>(y, x));
				v.push_back(gray);
			}
			

			maxValue = *max_element(v.begin(), v.end());
			minValue = *min_element(v.begin(), v.end());


			for (int index = 0; index < 5; index++){
				if (maxValue - minValue != 0)
					threshod = (double)(v[index] - minValue) / (maxValue - minValue);
				else
					threshod = 0;

				if (threshod<set_thresshold)
					binary_map[index].at<uchar>(y, x) = 0;
				else
					binary_map[index].at<uchar>(y, x) = 255;
			}			
			/*if (threshod!=0)
				cout << x << " " << y <<" "<<threshod<< endl;*/
		}
	}
	for (int index = 0; index < 5; index++)
		imwrite("binary_ntu_" + to_string(index) + ".jpg", binary_map[index]);
		

	///*double scale = 0.25;
	//resize(binary1, binary1, Size(img1.cols*scale, img1.rows*scale));WW
	//resize(img1, img1, Size(img1.cols*scale, img1.rows*scale));*/
	//

	namedWindow("HelloCV", WINDOW_AUTOSIZE);
	imshow("HelloCV", source_map[0]);
	imshow("binary0", binary_map[0]);
	//
	waitKey(0);
	destroyWindow("HelloCV");
	return 0;

}

string formatDobleValue(double val, int fixed) {
	auto str = std::to_string(val);
	return str.substr(0, str.find(".") + fixed + 1);
}
#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<algorithm>
#include <vector>

using namespace cv;
using namespace std;

string formatDobleValue(double val, int fixed);
bool if_fit_pattern(vector<int> pattern);
void GetGammaCorrection(Mat& src, Mat& dst, const float fGamma);
void GetBinaryMap(Mat src_map[5], float set_thresshold);
int _tmain(int argc, _TCHAR* argv[])
{
	

	string filename;
	Mat source_map[5];
	Mat binary_map[5];
	for (int index = 0; index < 5; index++){
		filename = "0um/img1-" + to_string(index + 1) + ".bmp";
		//filename = "img2-" + to_string(index+1) + ".bmp";
		source_map[index] = imread(filename, 2);
		binary_map[index] = source_map[index].clone();
	}	

	//imshow("source_map", source_map[0]);

	/*Mat gamma_img_map[5];
	float fGamma = 1 / 2.0;
	for (int index = 0; index < 5; index++)
		GetGammaCorrection(source_map[index], gamma_img_map[index], fGamma);*/
	//imshow("after_gamma_correction", gamma_img_map[0]);

	int minValue;
	int maxValue;
	int gray;
	double threshod;
	double set_thresshold = 0.6;
	bool binary_pattern[4];
	
	Mat noise_map = Mat(source_map[3].size(), source_map[3].type());

	for (int x = 0; x < source_map[0].cols; x++){
		for (int y = 0; y < 10; y++){

			vector<int> v = {};
			for (int index = 0; index < 5; index++){
				gray = (int)(source_map[index].at<uchar>(y, x));
				v.push_back(gray);
			}
			

			maxValue = *max_element(v.begin(), v.end());
			minValue = *min_element(v.begin(), v.end());

			vector<int> pattern = {};
			vector<int> front_four = {};
			vector<int> last_four = {};
			for (int index = 0; index < 5; index++){
				if (maxValue - minValue != 0)
					threshod = (double)(v[index] - minValue) / (maxValue - minValue);
				else
					threshod = 0;

				if (threshod < set_thresshold){
					binary_map[index].at<uchar>(y, x) = 0;
					pattern.push_back(0);
				}					
				else{
					binary_map[index].at<uchar>(y, x) = 255;
					pattern.push_back(1);
				}
					
			}	

			front_four.assign(pattern.begin(), pattern.end());
			front_four.pop_back();

			last_four.assign(pattern.begin(), pattern.end());
			last_four.erase(last_four.begin());
			
			bool result_front = if_fit_pattern(front_four);
			bool result_last = if_fit_pattern(last_four);

			if (result_front && result_last)
				noise_map.at<uchar>(y, x) = 0;
			else
				noise_map.at<uchar>(y, x) = 255;

			/*if (threshod!=0)
				cout << x << " " << y <<" "<<threshod<< endl;*/
		}
	}


	for (int index = 0; index < 5; index++)
		imwrite("binary_plane_th0.6_" + to_string(index) + ".jpg", binary_map[index]);
	
	
	
	imwrite("noise_map_plane_th0.6.jpg", noise_map);

	/*double scale = 0.25;
	resize(binary1, binary1, Size(img1.cols*scale, img1.rows*scale));WW
	resize(img1, img1, Size(img1.cols*scale, img1.rows*scale));*/
	

	namedWindow("HelloCV", WINDOW_AUTOSIZE);
	imshow("HelloCV", source_map[0]);
	imshow("binary0", binary_map[0]);
	//imshow("noise_map", noise_map);
	//
	waitKey(0);
	destroyWindow("HelloCV");
	return 0;

}

string formatDobleValue(double val, int fixed) {
	auto str = std::to_string(val);
	return str.substr(0, str.find(".") + fixed + 1);
}

bool if_fit_pattern(vector<int> pattern) {
	vector<int> i_vec1 = { 0, 1, 1, 0 };
	vector<int> i_vec2 = { 0, 0, 1, 1 };
	vector<int> i_vec3 = { 1, 0, 0, 1 };
	vector<int> i_vec4 = { 1, 1, 0, 0 };
	vector<int> i_vec5 = { 0, 1, 1, 1 };

	bool fit_pattern = false;
	if (pattern == i_vec1) fit_pattern = true;
	if (pattern == i_vec2) fit_pattern = true;
	if (pattern == i_vec3) fit_pattern = true;
	if (pattern == i_vec4) fit_pattern = true;
	if (pattern == i_vec5) fit_pattern = true;
	
	
	return fit_pattern;
}

void GetGammaCorrection(Mat& src, Mat& dst, const float fGamma)
{
	unsigned char bin[256];
	for (int i = 0; i < 256; ++i)
	{
		bin[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}
	dst = src.clone();
	const int channels = dst.channels();
	switch (channels)
	{
	case 1:
	{
			  MatIterator_<uchar> it, end;
			  for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
				  *it = bin[(*it)];
			  break;
	}
	case 3:
	{
			  MatIterator_<Vec3b> it, end;
			  for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
			  {
				  (*it)[0] = bin[((*it)[0])];
				  (*it)[1] = bin[((*it)[1])];
				  (*it)[2] = bin[((*it)[2])];
			  }
			  break;
	}
	}
}

#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include<algorithm>
#include <vector>
#include "tqdm.h"
#include<fstream>
#include <iostream>
using namespace cv;
using namespace std;

string formatDobleValue(double val, int fixed);
bool if_fit_pattern(vector<int> pattern);
void if_fit_certain_pattern(vector<int> pattern,int x,int y);
void GetGammaCorrection(Mat& src, Mat& dst, const float fGamma);
void GetBinaryMap(Mat src_map[5], float set_thresshold);

//set mouse event
static void onMouse(int event, int x, int y, int, void* userInput);

int width = 4096;
int height = 3000;
Mat source_map[5];
static int maze[4096][3000][5] = { -1 };
Mat noise_map;
Mat color_pattern = Mat(3000, 4096, CV_8UC3);
int _tmain(int argc, _TCHAR* argv[])
{
	ofstream myfile;
	myfile.open("test.txt");

	string filename;
	
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
	double set_thresshold = 0.5;
	bool binary_pattern[4];
	const int cols = source_map[0].cols;
	const int rows = source_map[0].rows;
	
	
	
	noise_map = Mat(source_map[3].size(), source_map[3].type());

	for (int y = 0; y < 100; y++){
		for (int x = 0; x < source_map[0].cols; x++){

	/*for (int x = 0; x < source_map[0].cols; x++){
		for (int y = 0; y < 500; y++){*/

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
					maze[x][y][index] = 0;
				}					
				else{
					binary_map[index].at<uchar>(y, x) = 255;
					pattern.push_back(1);
					maze[x][y][index] = 1;
				}
					
			}	

			front_four.assign(pattern.begin(), pattern.end());
			front_four.pop_back();

			last_four.assign(pattern.begin(), pattern.end());
			last_four.erase(last_four.begin());
			
			bool result_front = if_fit_pattern(front_four);
			bool result_last = if_fit_pattern(last_four);

			if_fit_certain_pattern(front_four,x,y);
			/*if (result_front){
				noise_map.at<uchar>(y, x) = 0;
			}
			else{
				myfile << x << " " << y << " :" << maze[x][y][0] << maze[x][y][1] << maze[x][y][2] << maze[x][y][3] << maze[x][y][4] << endl;
				noise_map.at<uchar>(y, x) = 255;
			}*/
			if (result_front || result_last)
				noise_map.at<uchar>(y, x) = 0;
			else
				noise_map.at<uchar>(y, x) = 255;

			/*if (threshod!=0)
				cout << x << " " << y <<" "<<threshod<< endl;*/
		}
	}

	myfile.close();
	for (int index = 0; index < 5; index++)
		imwrite("test_" + to_string(index) + ".jpg", binary_map[index]);
	
	
	
	imwrite("test.jpg", noise_map);

	/*double scale = 0.25;
	resize(binary1, binary1, Size(img1.cols*scale, img1.rows*scale));WW
	resize(img1, img1, Size(img1.cols*scale, img1.rows*scale));*/
	
	resize(noise_map, noise_map, Size(1920, 1080));
	namedWindow("HelloCV");

	setMouseCallback("HelloCV", onMouse);

	namedWindow("color_pattern");
	imshow("color_pattern", color_pattern);
	//imshow("binary0", binary_map[0]);
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



void if_fit_certain_pattern(vector<int> pattern,int x, int y) {
	vector<int> i_vec1 = { 1, 0, 1, 1 };
	

	bool fit_pattern = false;
	if (pattern == i_vec1) {

		try {
			color_pattern.at<Vec3b>(y, x)[0] = 0;
			color_pattern.at<Vec3b>(y, x)[1] = 0;
			color_pattern.at<Vec3b>(y, x)[2] = 255;
		}
		catch (std::exception &e) {
			cout << "exception: " << e.what() << "\n";
		}


		/*if (pattern == i_vec5) fit_pattern = true;*/
	}

	
}

bool if_fit_pattern(vector<int> pattern) {
	vector<int> i_vec1 = { 0, 1, 1, 0 };
	vector<int> i_vec2 = { 0, 0, 1, 1 };
	vector<int> i_vec3 = { 1, 0, 0, 1 };
	vector<int> i_vec4 = { 1, 1, 0, 0 };
	//vector<int> i_vec5 = { 0, 1, 1, 1 };

	bool fit_pattern = false;
	if (pattern == i_vec1) fit_pattern = true;
	if (pattern == i_vec2) fit_pattern = true;
	if (pattern == i_vec3) fit_pattern = true;
	if (pattern == i_vec4) fit_pattern = true;
	/*if (pattern == i_vec5) fit_pattern = true;*/
	
	
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

static void onMouse(int event, int x, int y, int, void* userInput)
{
	//只响应 鼠标左键按下事件;
	if (event != EVENT_LBUTTONDOWN) return;

	Mat* img = (Mat*)userInput;

	Point Pre_pt = Point(x+20, y+500);
	Point pattern_pt = Point(x + 20, y+520);
	char Txt_Point[80];
	char pattern[80];

	sprintf_s(Txt_Point, "(%d,%d)", x, y);
	sprintf_s(pattern, "(%d,%d,%d,%d,%d)", maze[x][y][0], maze[x][y][1], maze[x][y][2], maze[x][y][3], maze[x][y][4]);
	putText(noise_map, Txt_Point, Pre_pt,
		CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
	putText(noise_map, pattern, pattern_pt,
		CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
	//在点击坐标 绘制圆形;
	circle(noise_map, Point(x, y), 10, Scalar(0, 255, 0), 10);

	imshow("HelloCV", noise_map);
}
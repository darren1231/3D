#include "stdafx.h"
#include <opencv2/opencv.hpp>  
#include <math.h>
#include<fstream>
#include <iostream>
#include <ctime>

using namespace std;
using namespace cv;

void Conver2cloud(Mat src);
vector<float> loadtxt(string loadPath);
Mat read_zmap(string loadPath);
int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("20220718_only_0to1_noise/20220718_b2w_noise.jpg", 2);


	Mat Z_map = read_zmap("source/cloud/Z_map.xml");
	ofstream point_cloud_after_rmnoise;
	point_cloud_after_rmnoise.open("point_cloud_after_rmnoise.xyz");
	/*float height;
	int gray;
	for (int y = 0; y < Z_map.rows; y++){
		for (int x = 0; x < Z_map.cols; x++){

			height = (float)Z_map.at<float>(y, x);
			cout << x << " " << y << " " << Z_map.at<float>(y, x) << endl;
		}
	}*/
	int sum = 0;
	int sum_of_right = 0;
	float height;

	ofstream myfile;
	ofstream point_cloud_zmap;
	point_cloud_zmap.open("point_cloud_zmap.xyz");

	myfile.open("20220719_record_0to1_noise.txt");
	for (int y = 0; y < Z_map.rows; y++){
		for (int x = 0; x < Z_map.cols; x++){

			height = Z_map.at<float>(y, x);
			if ((int)img.at<uchar>(y, x) == 255){
				sum += 1;

				/*point_cloud_after_rmnoise << x << " " << y << " " << Z_map.at<float>(y, x) << endl;*/
				myfile << x << " " << y << " " << height << endl;
				if (height>100)
					sum_of_right+=1;
			}
			else{
				point_cloud_after_rmnoise << x << " " << y << " " << Z_map.at<float>(y, x) << endl;
			}
			/*point_cloud_zmap << x << " " << y << " " << height << endl;*/

			// Split the image into different channels

			//point_cloud << x << " " << y << " " << Z_map.at<float>(y, x) << rgbChannels[0](y, x) << endl;
		}
	}

	//Conver2cloud(img);

	myfile.close();
	point_cloud_after_rmnoise.close();
	point_cloud_zmap.close();
	cout << "sum of noise pixel: "<<sum << endl;
	cout << "sum of right noise pixel: " << sum_of_right << endl;



	if (img.empty()) return -1;
	namedWindow("HelloCV");
	imshow("HelloCV", img);
	waitKey(0);
	destroyWindow("HelloCV");
	return 0;

}

Mat read_zmap(string loadPath){

	const clock_t begin_time = clock();
	cv::FileStorage fs(loadPath, cv::FileStorage::READ);
	Mat Z_map;
	fs["X"] >> Z_map; // x contains gibberish
	cout << "after reading xml took(s)..." << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

	return Z_map;
}
vector<float> loadtxt(string loadPath)
{
	ifstream inFile(loadPath, ios::in);
	if (!inFile)
	{
		cout << "¶}±ÒÀÉ®×¥¢±Ñ¡I" << endl;
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
void Conver2cloud(Mat img){
	vector<float> src = loadtxt("source/cloud/point_cloud_ori.xyz");
	Mat z = Mat(1, 5044624, CV_32FC1);
	int n = 0;
	for (int i = 0; i < src.size(); i = i + 3){
		/*x.at<float>(0, n) = src[i];
		y.at<float>(0, n) = src[i + 1];*/
		z.at<float>(0, n) = src[i + 2];
		n++;
	}
	ofstream point_cloud, point_cloud_shaddow, point_cloud_fix_z;
	point_cloud.open("point_cloud_process.xyz");
	point_cloud_shaddow.open("point_cloud_shaddow.xyz");

	Mat Z_map = Mat(2054, 2456, CV_32FC1);
	int X = 0, Y = 0;
	for (int num = 0; num < 5044624; num++){

		Z_map.at<float>(Y, X) = -1 * z.at<float>(0, num);

		X++;
		if (X == 2456){
			X = 0;
			Y++;
		}
	}


	std::vector<cv::Mat> rgbChannels(3);
	split(img, rgbChannels);
	

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
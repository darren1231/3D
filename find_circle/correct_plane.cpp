#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/opencv.hpp>  
#include <math.h>
#include<fstream>
#include <stdio.h>
#include <ctime>


using namespace std;
using namespace cv;
using std::cout; using std::endl;
using std::setw; using std::vector;

void plane_matrix(float point_3d[4], float result[4]);
int main(){

	const clock_t begin_time = clock();
	cv::FileStorage fs("source/cloud/Z_map.xml", cv::FileStorage::READ);
	Mat Z_map;
	fs["X"] >> Z_map; // x contains gibberish
	cout << "after reading xml ..." << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

	ofstream point_cloud;
	point_cloud.open("20220727_correct_plane/20220727_plane_correction_sub.xyz");

	float height;
	float point_3d[4] = { 0 };
	float matrix_result[4] = { 0 };
	/*float plane_transition_matrix[4][4] = { { 0.602533, -0.797786, -0.022175, 0 },
	{ 0.796850, 0.602912, -0.039078, 0 },
	{ 0.044546, 0.005876, 0.998990, 0 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 } };*/

	float plane_transition_matrix[4][4] = { { -0.841437, 0.538830, 0.040573, 2592.037598 },
	{ -0.537246, -0.842277, 0.044014, 1582.230591 },
	{ 0.057889, 0.015237, 0.998207, -94.778191, },
	{ 0.000000, 0.000000, 0.000000, 1.0 } };

	/*float plane_transition_matrix[4][4] = { { 1, 0.538830, 0.040573, 2592.037598 },
	{ -0.537246, 1, 0.044014, 1582.230591 },
	{ 0.057889, 0.015237, 1, -94.778191, },
	{ 0.000000, 0.000000, 0.000000, 1.0 } };*/

	for (int y = 0; y < Z_map.rows; y++){
		for (int x = 0; x < Z_map.cols; x++){

			
				height = (float)Z_map.at<float>(y, x);
				point_3d[0] = (float)x;
				point_3d[1] = (float)y;
				point_3d[2] = height;
				point_3d[3] = 1.0;

				plane_matrix(point_3d, matrix_result);
				/*point_3d[0][0] = (float)x;
				point_3d[1][0] = (float)y;
				point_3d[2][0] = height;
				point_3d[3][0] = 1.0;			

				for (int m = 0; m<4; m++){
					for (int s = 0; s<1; s++){
						matrix_result[m][s] = 0;
						for (int n = 0; n<4; n++){
							matrix_result[m][s] += plane_transition_matrix[m][n] * point_3d[n][s];
						}
					}
				}*/
			
				/*for (int m = 0; m<4; m++){
					for (int s = 0; s<1; s++){
						cout << matrix_result[m][s] << "\t";
					}
					cout << endl;
				}*/

			point_cloud << matrix_result[0] << " " << matrix_result[1] << " " << matrix_result[2] << endl;

		}
	}

	point_cloud.close();
	
	
	

	system("pause");
	return EXIT_SUCCESS;
}

void plane_matrix(float point_3d[4], float matrix_result[4]){

	

	
	float plane_transition_matrix[4][4] = { { -0.841437, 0.538830, 0.040573, 2592.037598 },
	{ -0.537246, -0.842277, 0.044014, 1582.230591 },
	{ 0.057889, 0.015237, 0.998207, -94.778191, },
	{ 0.000000, 0.000000, 0.000000, 1.0 } };

	for (int m = 0; m<4; m++){
		/*for (int s = 0; s<1; s++){*/
			matrix_result[m] = 0;
			for (int n = 0; n<4; n++){
				matrix_result[m] += plane_transition_matrix[m][n] * point_3d[n];
			}
		//}
	}
	
}
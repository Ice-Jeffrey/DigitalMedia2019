#include<iostream>
#include<vector>
#include<algorithm>
#include<opencv2/opencv.hpp>
#include<cfloat>
using namespace std;
using namespace cv;

vector<int> FindSeam(vector<int>& seam, const Mat& energy_map) {
	//动态规划寻找能量最小的seam
	const int r = energy_map.rows;
	const int c = energy_map.cols;

	vector<vector<float>> dp_mat_(r, vector<float>(c));
	vector<vector<int>> dp_path_(r, vector<int>(c));
	// 第一行的路径能量直接用像素能量赋值
	for (int j = 0; j < c; j++) {
		dp_mat_[0][j] = energy_map.at<float>(0, j);
		dp_path_[0][j] = j;
	}
	// 向下遍历像素，更新 M[i][j]，记录路径
	for (int i = 1; i < r; i++) {
		for (int j = 0; j < c; ++j) {
			float energy_left_upper = j - 1 >= 0 ?
				dp_mat_[i - 1][j - 1] :
				DBL_MAX;
			float energy_right_upper = j + 1 < c ?
				dp_mat_[i - 1][j + 1] :
				DBL_MAX;
			float energy_upper = dp_mat_[i - 1][j];
			float energy_min = std::min(energy_upper,
				std::min(energy_left_upper, energy_right_upper));
			int parent_idx = j;
			if (std::fabs(energy_min - energy_left_upper) < 1e-6) {
				parent_idx = j - 1;
			}
			else if (std::fabs(energy_min - energy_right_upper) < 1e-6) {
				parent_idx = j + 1;
			}
			dp_mat_[i][j] = energy_map.at<float>(i, j) +
				energy_min;
			dp_path_[i][j] = parent_idx;
		}
	}

	//更新seam
	// 找最后一行里对应最小路径能量和的像素点
	vector<float>::iterator smallest = std::min_element(dp_mat_[r - 1].begin(), dp_mat_[r - 1].end());
	int col_idx = std::distance(dp_mat_[r - 1].begin(), smallest);
	
	// 回溯得到完整路径
	seam[r - 1] = col_idx;
	for (int k = r - 1; k > 0; --k) {
		col_idx = dp_path_[k][col_idx];
		seam[k - 1] = col_idx;
	}

	return seam;
}

void RemoveSeam(vector<int>& seam, Mat &temp) {
	if (temp.rows <= 1 || temp.cols <= 1) {
		return;
	}
	cv::Mat carved_img(temp.rows, temp.cols - 1, temp.type(), cv::Scalar(0, 0, 0));
	for (int i = 0; i < temp.rows; ++i) {
		for (int j = 0; j < seam[i]; ++j) {
			carved_img.at<cv::Vec3b>(i, j) = temp.at<cv::Vec3b>(i, j);
		}
		for (int j = seam[i]; j < carved_img.cols; ++j) {
			carved_img.at<cv::Vec3b>(i, j) = temp.at<cv::Vec3b>(i, j + 1);
		}
	}
	temp = carved_img.clone();
}

int main() {
	//读入图片并进行初始化
	Mat src = imread("C:/Users/Jeffrey/Pictures/Seam-2.jpg");
	imshow("Original Image", src);
	waitKey(0);
	Mat src_gray;
	Mat sobel_x_map, sobel_y_map;
	Mat energy_map;
	Mat temp = src.clone();

	//寻找seam
	for (int i = 0; i < 150; i++) {
		//Sobel算子绘制出能量图energy_map
		cv::cvtColor(temp, src_gray, cv::COLOR_BGR2GRAY);
		cv::Sobel(src_gray, sobel_x_map, CV_32F, 1, 0, 3);
		cv::convertScaleAbs(sobel_x_map, sobel_x_map);
		cv::Sobel(src_gray, sobel_y_map, CV_32F, 0, 1, 3);
		cv::convertScaleAbs(sobel_y_map, sobel_y_map);
		cv::addWeighted(sobel_x_map, 0.5, sobel_y_map, 0.5, 0, energy_map);
		vector<int> seam(energy_map.rows, 0);
		seam = FindSeam(seam, energy_map);
		for (int i = 0; i < temp.rows; ++i) {
			src.at<cv::Vec3b>(i, seam[i])[0] = 0;
			src.at<cv::Vec3b>(i, seam[i])[1] = 0;
			src.at<cv::Vec3b>(i, seam[i])[2] = 255;
		}

		RemoveSeam(seam, temp);
	}
	
	imshow("seam image", temp);
	waitKey(0);

	return 0;
}
#pragma once

#include<iostream>
#include<opencv2/opencv.hpp>
#include<cmath>
#include<algorithm>
#include<vector>
using namespace cv;
using namespace std;

// 均值滤波手动实现
void Box(cv::Mat src,cv::Mat &dst, int kernal_width, int kernal_height) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	for (int i = kernal_width / 2; i < src.rows - kernal_width / 2; i++) {
		for (int j = kernal_height / 2; j < src.cols - kernal_height / 2; j++) {
			for (int k = 0; k < 3; k++) {
				double sum = 0;
				for (int r = i - kernal_width / 2; r <= i + kernal_width / 2; r++) {
					for (int c = j - kernal_height / 2; c <= j + kernal_height / 2; c++) {
						sum += src.at<Vec3b>(r, c)[k];
					}
				}
				double average = round(sum / ((double)kernal_width * kernal_height));
				dst.at<Vec3b>(i, j)[k] = average;
			}
		}
	}
}

// 中值滤波手动实现
void Median(cv::Mat src, cv::Mat& dst, int size) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	for (int i = size / 2; i < src.rows - size / 2; i++) {
		for (int j = size / 2; j < src.cols - size / 2; j++) {
			for (int k = 0; k < 3; k++) {
				unsigned char median;
				vector<unsigned char> vec;
				//compute the median value
				for (int r = i - size / 2; r <= i + size / 2; r++) {
					for (int c = j - size / 2; c <= j + size / 2; c++) {
						vec.push_back(src.at<Vec3b>(r, c)[k]);
					}
				}
				sort(vec.begin(), vec.end());
				if ((size % 2) == 0)
					median = round((vec[size * size / 2] + vec[(double)(size * size / 2) - 1]) / 2);
				else
					median = vec[size * size / 2];
				dst.at<Vec3b>(i, j)[k] = median;
			}
		}
	}
}

// 最大值滤波手动实现
void Max(cv::Mat src, cv::Mat& dst, int kernal_width, int kernal_height) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	for (int i = kernal_width / 2; i < src.rows - kernal_width / 2; i++) {
		for (int j = kernal_height / 2; j < src.cols - kernal_height / 2; j++) {
			for (int k = 0; k < 3; k++) {
				unsigned char max;
				vector<unsigned char> vec;
				//compute the median value
				for (int r = i - kernal_width / 2; r <= i + kernal_width / 2; r++) {
					for (int c = j - kernal_height / 2; c <= j + kernal_height / 2; c++) {
						vec.push_back(src.at<Vec3b>(r, c)[k]);
					}
				}
				sort(vec.begin(), vec.end());
				max = vec[vec.size() - 1];
				dst.at<Vec3b>(i, j)[k] = max;
			}
		}
	}
}

// 最小值滤波手动实现
void Min(cv::Mat src, cv::Mat& dst, int kernal_width, int kernal_height) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	for (int i = kernal_width / 2; i < src.rows - kernal_width / 2; i++) {
		for (int j = kernal_height / 2; j < src.cols - kernal_height / 2; j++) {
			for (int k = 0; k < 3; k++) {
				unsigned char min;
				vector<unsigned char> vec;
				//compute the median value
				for (int r = i - kernal_width / 2; r <= i + kernal_width / 2; r++) {
					for (int c = j - kernal_height / 2; c <= j + kernal_height / 2; c++) {
						vec.push_back(src.at<Vec3b>(r, c)[k]);
					}
				}
				sort(vec.begin(), vec.end());
				min = vec[0];
				dst.at<Vec3b>(i, j)[k] = min;
			}
		}
	}
}

double GaussianWeight(int x, int y, float sigmaX, float sigmaY = 0.0) {
	if (sigmaY == 0) sigmaY = sigmaX;
	return exp(-(x * x * sigmaY * sigmaY + y * y * sigmaX * sigmaX) / (2 * sigmaX * sigmaX * sigmaY * sigmaY)) 
			/ (2 * 3.14 * sigmaX * sigmaY);
}

// 高斯滤波手动实现
void Gaussian(cv::Mat src, cv::Mat& dst,int kernal_size, float sigmaX, float sigmaY = 0.0) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	if (sigmaX == 0.0) sigmaX = 0.3 * (((double)kernal_size - 1) * 0.5 - 1) + 0.8;
	if (sigmaY == 0.0)	sigmaY = sigmaX;
	int height = kernal_size / 2;
	int width = kernal_size / 2;
	double** weights;
	weights = new double*[kernal_size];
	for (int i = 0; i < kernal_size; i++)
		weights[i] = new double[kernal_size];
	for (int r = -height; r <= height; r++) {
		for (int c = -width; c <= width; c++) {
			*(*(weights + width + r) + height + c) = GaussianWeight(r, c, sigmaX, sigmaY);
		}
	}
	double sum = 0;
	for (int i = height; i < src.rows - height; i++) {
		for (int j = width; j < src.cols - width; j++) {
			for (int k = 0; k < src.channels(); k++) {
				double sum = 0;
				for (int r = -height; r <= height; r++) {
					for (int c = -width; c <= width; c++) {
						sum += weights[width + r][height + c] * src.at<Vec3b>(i + r, j + c)[k];
					}
				}
				dst.at<Vec3b>(i, j)[k] = sum;
			}
		}
	}
}

// 双边滤波手动实现
void Bilateral(cv::Mat src, cv::Mat& dst, int d, float sigmaColor, float sigmaSpace) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	int height = d / 2;
	int width = d / 2;
	double sum = 0;
	double** weights;
	weights = new double* [d];
	for (int i = 0; i < d; i++)
		weights[i] = new double[d];
	for (int r = -height; r <= height; r++) {
		for (int c = -width; c <= width; c++) {
			*(*(weights + width + r) + height + c) = exp(-(r * r + c * c) / (2 * sigmaSpace * sigmaSpace));
		}
	}
	for (int i = width; i < src.rows - width; i++) {
		for (int j = height; j < src.cols - height; j++) {
			for (int k = 0; k < src.channels(); k++) {
				double sum = 0, weight_sum = 0;
				for (int r = -width; r <= width; r++) {
					for (int c = -height; c <= height; c++) {
						double weight = weights[width + r][height + c] * exp(
							- (src.at<Vec3b>(i + r, j + c)[k] - src.at<Vec3b>(i, j)[k]) * 
							(src.at<Vec3b>(i + r, j + c)[k] - src.at<Vec3b>(i, j)[k]) / 
							(2 * sigmaColor * sigmaColor)
						);
						weight_sum += weight;
						double result = weight * src.at<Vec3b>(i + r, j + c)[k];
						sum += result;
					}
				}
				dst.at<Vec3b>(i, j)[k] = round(sum * 1.0 / weight_sum);
			}
		}
	}
}
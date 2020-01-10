#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/ximgproc/edge_filter.hpp>	//导向图滤波的官方算法
#include<vector>
#include<algorithm>
#include<cmath>
using namespace cv;
using namespace std;

typedef struct Node {
	int i, j;
	uchar e;
} Class;

bool cmp(const Class &c1, const Class &c2) {
	return (c1.e > c2.e);
}

template<typename T>
T range(T begin, T end, T value) {
	if (value < begin)
		value = begin;
	if (value > end)
		value = end;
	return value;
}

Mat DarkChannel_uchar(Mat src) {
	Mat dark = Mat::zeros(src.rows, src.cols, CV_8U);
	for(int i=0; i<src.rows; i++) 
		for (int j = 0; j < src.cols; j++) {
			dark.at<uchar>(i, j) = min(
				min(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i, j)[1]),
				min(src.at<Vec3b>(i, j)[1], src.at<Vec3b>(i, j)[2])
			);
		}
	return dark;
}

Mat DarkChannel_double(Mat src) {
	Mat dark = Mat::zeros(src.rows, src.cols, CV_64FC1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dark.at<double>(i, j) = min(
				min(src.at<Vec3d>(i, j)[0], src.at<Vec3d>(i, j)[1]),
				min(src.at<Vec3d>(i, j)[1], src.at<Vec3d>(i, j)[2])
			);
		}
	}
	return dark;
}

void MinFilter_uchar(Mat src, Mat& dst, int kernal_width, int kernal_height) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	for (int i = kernal_width / 2; i < src.rows - kernal_width / 2; i++) {
		for (int j = kernal_height / 2; j < src.cols - kernal_height / 2; j++) {
			unsigned char min;
			vector<unsigned char> vec;
			for (int r = i - kernal_width / 2; r <= i + kernal_width / 2; r++) {
				for (int c = j - kernal_height / 2; c <= j + kernal_height / 2; c++) {
						vec.push_back(src.at<uchar>(r, c));
				}
			}
			sort(vec.begin(), vec.end());
			min = vec[0];
			dst.at<uchar>(i, j) = min;
		}
	}
}

void MinFilter_double(Mat src, Mat& dst, int kernal_width, int kernal_height) {
	if (dst.empty())
		dst = cv::Mat(src.rows, src.cols, src.type());
	for (int i = kernal_width / 2; i < src.rows - kernal_width / 2; i++) {
		for (int j = kernal_height / 2; j < src.cols - kernal_height / 2; j++) {
			double min;
			vector<double> vec;
			for (int r = i - kernal_width / 2; r <= i + kernal_width / 2; r++) {
				for (int c = j - kernal_height / 2; c <= j + kernal_height / 2; c++) {
					vec.push_back(src.at<double>(r, c));
				}
			}
			sort(vec.begin(), vec.end());
			min = vec[0];
			dst.at<double>(i, j) = min;
		}
	}
}

vector<double> AComputing(Mat src, Mat dark) {
	int count = round(src.cols * src.rows * 1.0 / 1000);
	vector<double> A;

	//遍历暗通道
	vector<Class> v;
	for (int i = 0; i < dark.rows; i++) 
		for (int j = 0; j < dark.cols; j++) {
			Class node;
			node.i = i;
			node.j = j;
			node.e = dark.at<uchar>(i, j);
			v.push_back(node);
		}
			
	//选出暗通道前0.1%亮度的像素对应位置的原图像素
	sort(v.begin(), v.end(), cmp);
	vector<uchar> temp1, temp2, temp3;
	for (int i = 0; i < count; i++) {
		int x = v[i].i;
		int y = v[i].j;
		temp1.push_back(src.at<Vec3b>(x, y)[0]);
		temp2.push_back(src.at<Vec3b>(x, y)[1]);
		temp3.push_back(src.at<Vec3b>(x, y)[2]);
	}
	//对三个通道分别进行排序
	double sum = 0;
	for (int i = 0; i < count; i++)
		sum += temp1[i];
	A.push_back(sum * 1.0 / count);
	sum = 0;
	for (int i = 0; i < count; i++)
		sum += temp2[i];
	A.push_back(sum * 1.0 / count);
	sum = 0;
	for (int i = 0; i < count; i++)
		sum += temp3[i];
	A.push_back(sum * 1.0 / count);

	for (int i = 0; i < 3; i++)
		A[i] = range(double(0), 255 * 0.8, A[i]);

	return A;
}



int main() {
	//读入图片
	Mat src;
	src = imread("C:/Users/Jeffrey/Pictures/Haze.jpeg");
	if (src.empty()) {
		cout << "Failed to load the image!" << endl;
		return -1;
	}

	//计算暗通道
	Mat dark1 = DarkChannel_uchar(src);
	//进行最小值滤波
	Mat dark2;
	MinFilter_uchar(dark1, dark2, 11, 11);
	imshow("test", dark2);
	waitKey(0);
	destroyWindow("test");

	//计算A值
	vector<double> A = AComputing(src, dark2);

	//对原图像归一化
	Mat temp = Mat::zeros(src.rows, src.cols, CV_64FC3);
	for (int i = 0; i < temp.rows; i++) {
		for (int j = 0; j < temp.cols; j++) {
			for (int k = 0; k < 3; k++) {
				temp.at<Vec3d>(i, j)[k] = double(src.at<Vec3b>(i, j)[k] * 1.0 / A[k]);
			}
		}
	}
	//取暗通道
	Mat dark3 = DarkChannel_double(temp);
	//最小值滤波
	Mat dark4;
	MinFilter_double(dark3, dark4, 11, 11);
	//计算t
	double omega = 0.9;
	Mat t = Mat::zeros(dark4.rows, dark4.cols, CV_64FC1);
	for (int i = 0; i < t.rows; i++)
		for (int j = 0; j < t.cols; j++) {
			t.at<double>(i, j) = double(1 - omega * dark4.at<double>(i, j));
		}
		 	
	//求出原图像
	Mat Haze_free = Mat::zeros(src.rows, src.cols, src.type());
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			for (int k = 0; k < 3; k++) {
				double temp1 = double(src.at<Vec3b>(i, j)[k] - A[k]);
				double value = A[k] + temp1 * 1.0 / max(double(t.at<double>(i, j)), 0.1);
				if (value > 255)
					value = 255;
				if (value < 0)
					value = 0;
				Haze_free.at<Vec3b>(i, j)[k] = value;
			}
		}
	}

	//最后对原图像进行导向图滤波
	Mat deFog;
	ximgproc::guidedFilter(src, Haze_free, deFog, 20, 0.001);

	//显示原图
	imshow("source", src);
	waitKey(0);
	imshow("Haze_Free", deFog);
	waitKey(0);

	return 0;
}
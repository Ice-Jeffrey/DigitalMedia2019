#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "filter.h"

using namespace std;
using namespace cv;

void fun(cv::Mat src, cv::Mat dst, int function) {
	int kernal_width, kernal_height;
	if (function == 1) {
		cout << "请输入滤波核的高度和宽度：";
		cin >> kernal_height >> kernal_width;
		//均值滤波
		Box(src, dst, kernal_width, kernal_height);
		namedWindow("均值滤波");
		cv::imshow("均值滤波", dst);
		cv::waitKey(0);
		destroyWindow("均值滤波");
	}
	else if (function == 2) {
		int size;
		cout << "请输入滤波核的边长：";
		cin >> size;
		//中值滤波
		Median(src, dst, size);
		namedWindow("中值滤波");
		cv::imshow("中值滤波", dst);
		cv::waitKey(0);
		destroyWindow("中值滤波");
	}
	else if (function == 3) {
		cout << "请输入滤波核的高度和宽度：";
		cin >> kernal_height >> kernal_width;
		//最小值滤波
		Min(src, dst, kernal_width, kernal_height);
		namedWindow("最小值滤波");
		cv::imshow("最小值滤波", dst);
		cv::waitKey(0);
		destroyWindow("最小值滤波");
	}
	else if (function == 4) {
		cout << "请输入滤波核的高度和宽度：";
		cin >> kernal_height >> kernal_width;
		//最大值滤波
		Max(src, dst, kernal_width, kernal_height);
		namedWindow("最大值滤波");
		cv::imshow("最大值滤波", dst);
		cv::waitKey(0);
		destroyWindow("最大值滤波");
	}
	else if (function == 5) {
		int kernal_size;
		cout << "请输入滤波核的边长：";
		cin >> kernal_size;
		//高斯滤波
		Gaussian(src, dst, kernal_size, 1.5);
		namedWindow("高斯滤波");
		cv::imshow("高斯滤波", dst);
		cv::waitKey(0);
		destroyWindow("高斯滤波");
	}
	else if (function == 6) {
		//双边滤波
		int d;
		double sigmaColor, sigmaSpace;
		cout << "请输入像素邻域的直径范围、色彩的sigma值、空间的sigma值：";
		cin >> d >> sigmaColor >> sigmaSpace;
		//bilateralFilter(src, dst, d, sigmaColor, sigmaSpace);
		Bilateral(src, dst, d, sigmaColor, sigmaSpace);
		namedWindow("双边滤波");
		cv::imshow("双边滤波", dst);
		cv::waitKey(0);
		destroyWindow("双边滤波");
	}
}

int main() {
	cv::Mat src, dst;
	src = cv::imread("C:/Users/Jeffrey/Pictures/Lena.jpg");
	if (src.empty()) {
		cout << "Fail to load the image" << endl;
	}

	cout << "请输入要实现的功能：" << endl
		<< "1. 均值滤波" << endl
		<< "2. 中值滤波" << endl
		<< "3. 最小值滤波" << endl
		<< "4. 最大值滤波" << endl
		<< "5. 高斯滤波" << endl
		<< "6. 双边滤波" << endl
		<< "7. 退出程序" << endl << endl;

	int function;
	while (cin >> function) {
		if (function == 7)	break;
		fun(src, dst, function);
	}

	return 0;
}
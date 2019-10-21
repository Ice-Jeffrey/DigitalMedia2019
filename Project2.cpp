#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void fun(cv::Mat src, cv::Mat dst, int function) {
	cout << "请输入滤波核的长度：";
	int size;
	cin >> size;
	if (function == 1) {
		//简单模糊
		blur(src, dst, cv::Size(size, size));
		namedWindow("简单模糊");
		cv::imshow("简单模糊", dst);
		cv::waitKey(0);
		destroyWindow("简单模糊");
	}
	else if (function == 2) {
		//均值滤波
		boxFilter(src, dst, 8, cv::Size(size, size));
		namedWindow("均值滤波");
		cv::imshow("均值滤波", dst);
		cv::waitKey(0);
		destroyWindow("均值滤波");
	}
	else if (function == 3) {
		//中值滤波
		medianBlur(src, dst, size); 
		namedWindow("中值滤波");
		cv::imshow("中值滤波", dst);
		cv::waitKey(0);
		destroyWindow("中值滤波");
	}
	else if (function == 4) {
		//高斯滤波
		GaussianBlur(src, dst, cv::Size(size, size), 0.0);
		namedWindow("高斯滤波");
		cv::imshow("高斯滤波", dst);
		cv::waitKey(0);
		destroyWindow("高斯滤波");
	}
	else if (function == 5) {
		//双边滤波
		bilateralFilter(src, dst, 32, 183, 188);
		namedWindow("双边滤波");
		cv::imshow("双边滤波", dst);
		cv::waitKey(0);
		destroyWindow("双边滤波");
	}
}

int main() {
	cv::Mat src, dst;
	src = cv::imread("C:/Users/Jeffrey/Pictures/Jerome.jpg");
	if (src.empty()) {
		cout << "Fail to load the image" << endl;
	}
	
	cout << "请输入要实现的功能：" << endl
		<< "1. 简单模糊" << endl
		<< "2. 均值滤波" << endl
		<< "3. 中值滤波" << endl
		<< "4. 高斯滤波" << endl
		<< "5. 双边滤波" << endl
		<< "6. 退出程序" << endl << endl;

	int function;
	while (cin >> function) {
		if (function == 6)	break;
		fun(src, dst, function);
	}	

	return 0;
}
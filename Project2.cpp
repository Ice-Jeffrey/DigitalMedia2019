#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;

void fun(cv::Mat src, cv::Mat dst, int function) {
	if (function == 1) {
		//实现简单模糊
		blur(src, dst, cv::Size(9, 9));
		cv::imshow("效果", dst);
		cv::waitKey(0);
	}
	else if (function == 2) {
		//实现均值滤波
		boxFilter(src, dst, 8, cv::Size(9, 9));
		cv::imshow("效果", dst);
		cv::waitKey(0);
	}
	else if (function == 3) {
		//实现中值滤波
		medianBlur(src, dst, 9); //Kernalcv::Size(3, 3));
		cv::imshow("效果", dst);
		cv::waitKey(0);
	}
	else if (function == 4) {
		//实现高斯滤波
		GaussianBlur(src, dst, cv::Size(9, 9), 0.0);
		cv::imshow("效果", dst);
		cv::waitKey(0);
	}
	else if (function == 5) {
		//实现双边滤波
		bilateralFilter(src, dst, -1, 0.0, 150);
		cv::imshow("效果", dst);
		cv::waitKey(0);
	}
}

int main() {
	cv::Mat src, dst;
	src = cv::imread("C:/Users/Jeffrey/Pictures/Jerome.jpg");
	if (src.empty()) {
		cout << "Fail to load the image" << endl;
	}
	//cv::waitKey(0);

	//int function;
	/*cout << "请输入想要实现的滤波效果：" << endl
		<< "1: 简单模糊" << endl
		<< "2: 均值滤波" << endl
		<< "3: 中值滤波" << endl
		<< "4: 高斯滤波" << endl
		<< "5: 双边滤波" << endl
		<< "6: 退出" << endl;*/

	//cv::namedWindow("效果", WINDOW_AUTOcv::Size);
	/*cv::namedWindow("source", cv::WINDOW_AUTOSIZE);
	cv::imshow("source", src);
	cv::waitKey(0);*/
	bilateralFilter(src, dst, -1, 0.0, 50);
	cv::imshow("效果", dst);
	cv::waitKey(0);

	return 0;
}
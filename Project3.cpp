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
		cout << "�������˲��˵ĸ߶ȺͿ�ȣ�";
		cin >> kernal_height >> kernal_width;
		//��ֵ�˲�
		Box(src, dst, kernal_width, kernal_height);
		namedWindow("��ֵ�˲�");
		cv::imshow("��ֵ�˲�", dst);
		cv::waitKey(0);
		destroyWindow("��ֵ�˲�");
	}
	else if (function == 2) {
		int size;
		cout << "�������˲��˵ı߳���";
		cin >> size;
		//��ֵ�˲�
		Median(src, dst, size);
		namedWindow("��ֵ�˲�");
		cv::imshow("��ֵ�˲�", dst);
		cv::waitKey(0);
		destroyWindow("��ֵ�˲�");
	}
	else if (function == 3) {
		cout << "�������˲��˵ĸ߶ȺͿ�ȣ�";
		cin >> kernal_height >> kernal_width;
		//��Сֵ�˲�
		Min(src, dst, kernal_width, kernal_height);
		namedWindow("��Сֵ�˲�");
		cv::imshow("��Сֵ�˲�", dst);
		cv::waitKey(0);
		destroyWindow("��Сֵ�˲�");
	}
	else if (function == 4) {
		cout << "�������˲��˵ĸ߶ȺͿ�ȣ�";
		cin >> kernal_height >> kernal_width;
		//���ֵ�˲�
		Max(src, dst, kernal_width, kernal_height);
		namedWindow("���ֵ�˲�");
		cv::imshow("���ֵ�˲�", dst);
		cv::waitKey(0);
		destroyWindow("���ֵ�˲�");
	}
	else if (function == 5) {
		int kernal_size;
		cout << "�������˲��˵ı߳���";
		cin >> kernal_size;
		//��˹�˲�
		Gaussian(src, dst, kernal_size, 1.5);
		namedWindow("��˹�˲�");
		cv::imshow("��˹�˲�", dst);
		cv::waitKey(0);
		destroyWindow("��˹�˲�");
	}
	else if (function == 6) {
		//˫���˲�
		int d;
		double sigmaColor, sigmaSpace;
		cout << "���������������ֱ����Χ��ɫ�ʵ�sigmaֵ���ռ��sigmaֵ��";
		cin >> d >> sigmaColor >> sigmaSpace;
		//bilateralFilter(src, dst, d, sigmaColor, sigmaSpace);
		Bilateral(src, dst, d, sigmaColor, sigmaSpace);
		namedWindow("˫���˲�");
		cv::imshow("˫���˲�", dst);
		cv::waitKey(0);
		destroyWindow("˫���˲�");
	}
}

int main() {
	cv::Mat src, dst;
	src = cv::imread("C:/Users/Jeffrey/Pictures/Lena.jpg");
	if (src.empty()) {
		cout << "Fail to load the image" << endl;
	}

	cout << "������Ҫʵ�ֵĹ��ܣ�" << endl
		<< "1. ��ֵ�˲�" << endl
		<< "2. ��ֵ�˲�" << endl
		<< "3. ��Сֵ�˲�" << endl
		<< "4. ���ֵ�˲�" << endl
		<< "5. ��˹�˲�" << endl
		<< "6. ˫���˲�" << endl
		<< "7. �˳�����" << endl << endl;

	int function;
	while (cin >> function) {
		if (function == 7)	break;
		fun(src, dst, function);
	}

	return 0;
}
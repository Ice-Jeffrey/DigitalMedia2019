#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;

void fun(cv::Mat src, cv::Mat dst, int function) {
	if (function == 1) {
		//ʵ�ּ�ģ��
		blur(src, dst, cv::Size(9, 9));
		cv::imshow("Ч��", dst);
		cv::waitKey(0);
	}
	else if (function == 2) {
		//ʵ�־�ֵ�˲�
		boxFilter(src, dst, 8, cv::Size(9, 9));
		cv::imshow("Ч��", dst);
		cv::waitKey(0);
	}
	else if (function == 3) {
		//ʵ����ֵ�˲�
		medianBlur(src, dst, 9); //Kernalcv::Size(3, 3));
		cv::imshow("Ч��", dst);
		cv::waitKey(0);
	}
	else if (function == 4) {
		//ʵ�ָ�˹�˲�
		GaussianBlur(src, dst, cv::Size(9, 9), 0.0);
		cv::imshow("Ч��", dst);
		cv::waitKey(0);
	}
	else if (function == 5) {
		//ʵ��˫���˲�
		bilateralFilter(src, dst, -1, 0.0, 150);
		cv::imshow("Ч��", dst);
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
	/*cout << "��������Ҫʵ�ֵ��˲�Ч����" << endl
		<< "1: ��ģ��" << endl
		<< "2: ��ֵ�˲�" << endl
		<< "3: ��ֵ�˲�" << endl
		<< "4: ��˹�˲�" << endl
		<< "5: ˫���˲�" << endl
		<< "6: �˳�" << endl;*/

	//cv::namedWindow("Ч��", WINDOW_AUTOcv::Size);
	/*cv::namedWindow("source", cv::WINDOW_AUTOSIZE);
	cv::imshow("source", src);
	cv::waitKey(0);*/
	bilateralFilter(src, dst, -1, 0.0, 50);
	cv::imshow("Ч��", dst);
	cv::waitKey(0);

	return 0;
}
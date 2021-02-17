#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/edge_filter.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace cv;
using namespace std;

typedef struct Node {
	int i, j;
	uchar e;
} Class;

bool cmp(const Class &c1, const Class &c2) {
	return (c1.e > c2.e);
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

// 计算图片中对应的A的值
vector<double> AComputing(Mat src, Mat dark) {
	int count = round(src.cols * src.rows * 1.0 / 1000);
	vector<double> A;

	// 对暗通道图像的亮度进行排序
	vector<Class> v;
	for (int i = 0; i < dark.rows; i++) 
		for (int j = 0; j < dark.cols; j++) {
			Class node;
			node.i = i;
			node.j = j;
			node.e = dark.at<uchar>(i, j);
			v.push_back(node);
		}
	sort(v.begin(), v.end(), cmp);

	vector<uchar> temp1, temp2, temp3;
	for (int i = 0; i < count; i++) {
		int x = v[i].i;
		int y = v[i].j;
		temp1.push_back(src.at<Vec3b>(x, y)[0]);
		temp2.push_back(src.at<Vec3b>(x, y)[1]);
		temp3.push_back(src.at<Vec3b>(x, y)[2]);
	}

	// 寻找各个通道的A值
	sort(temp1.begin(), temp1.end());
	sort(temp2.begin(), temp2.end());
	sort(temp3.begin(), temp3.end());
	A.push_back(temp1[count-1]);
	A.push_back(temp2[count-1]);
	A.push_back(temp3[count-1]);

	return A;
}



int main() {
	// 读取需要进行去雾的图片
	Mat src;
	src = imread("./img/Haze.png");
	if (src.empty()) {
		cout << "Failed to load the image!" << endl;
		return -1;
	}

	// 求得暗通道图片
	Mat dark1 = DarkChannel_uchar(src);
	// 对暗通道进行最小值滤波
	Mat dark2;
	MinFilter_uchar(dark1, dark2, 11, 11);

	// 测试输出滤波之后的暗通道
	// imshow("darkchannel", dark2);
	// waitKey(0);
	// destroyWindow("darkchannel");

	// 求出对应的A值
	vector<double> A = AComputing(src, dark2);

	Mat temp = Mat::zeros(src.rows, src.cols, CV_64FC3);
	for (int i = 0; i < temp.rows; i++) {
		for (int j = 0; j < temp.cols; j++) {
			for (int k = 0; k < 3; k++) {
				temp.at<Vec3d>(i, j)[k] = double(src.at<Vec3b>(i, j)[k] * 1.0 / A[k]);
			}
		}
	}
	Mat dark3 = DarkChannel_double(temp);
	Mat dark4;
	MinFilter_double(dark3, dark4, 11, 11);
	
	// 求对应的t值
	double omega = 0.9;
	Mat t = Mat::zeros(dark4.rows, dark4.cols, CV_64FC1);
	for (int i = 0; i < t.rows; i++)
		for (int j = 0; j < t.cols; j++) {
			t.at<double>(i, j) = double(1 - omega * dark4.at<double>(i, j));
		}
		 	
	// 进行去雾计算
	Mat Haze_free = Mat::zeros(src.rows, src.cols, src.type());
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			for (int k = 0; k < 3; k++) {
				double temp1 = double(src.at<Vec3b>(i, j)[k]) - A[k];
				double value = A[k] + temp1 * 1.0 / double(t.at<double>(i, j));
				if (value > 255)
					value = 255;
				if (value < 0)
					value = 0;
				Haze_free.at<Vec3b>(i, j)[k] = value;
			}
		}
	}

	// 进行导向图滤波
	Mat deFog;
	ximgproc::guidedFilter(src, Haze_free, deFog, 20, 0.001);

	// 对比去雾前后效果
	imshow("source", src);
	waitKey(0);
	imshow("Haze_Free", deFog);
	waitKey(0);

	return 0;
}
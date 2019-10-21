#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
using namespace cv;
using std::cout;

const int alpha_slider_max = 100;
const int beta_slider_max = 100;
const int saturation_slider_max = 255;
int alpha_slider;
int beta_slider;
int saturation_slider;
double alpha;
double beta;
double saturation;

Mat src;
Mat dst;

static void on_trackbar_alpha(int, void*) {
	alpha = alpha_slider * 3.0 / alpha_slider_max;
	src.convertTo(dst, -1, alpha, beta);
	imshow("Brightness and Contrastness", dst);
}

static void on_trackbar_beta(int, void*) {
	beta = beta_slider;
	src.convertTo(dst, -1, alpha, beta);
	imshow("Brightness and Contrastness", dst);
}

static void HSVadjust(int, void*) {
	Mat image_hsv;
	saturation = saturation_slider;
	cvtColor(src, image_hsv, CV_BGR2HSV);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//saturation_slider = image_hsv.at<Vec3b>(i, j)[1];
			image_hsv.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(
				image_hsv.at<Vec3b>(i, j)[1] + saturation
				);
		}
	}
	cvtColor(image_hsv, dst, CV_HSV2BGR);
	imshow("Brightness and Contrastness", dst);
}

int main(void)
{
	src = imread("F:\\Codes\\DigitalMedia2019\\Project_I\\Jerome.jpg");
	if (src.empty()) { cout << "Error loading source image! \n"; return -1; }

	namedWindow("Brightness and Contrastness", WINDOW_AUTOSIZE); // Create Window
	
	//设定对比度初始值为50
	alpha_slider = 50;
	char alphaName[50];
	sprintf_s(alphaName, "对比度 %d", alpha_slider_max);
	createTrackbar(alphaName, "Brightness and Contrastness", &alpha_slider, alpha_slider_max, on_trackbar_alpha);
	on_trackbar_alpha(alpha_slider, 0);

	//设定亮度初始值为50
	beta_slider = 50;
	char betaName[50];
	sprintf_s(betaName, "亮度 %d", beta_slider_max);
	createTrackbar(betaName, "Brightness and Contrastness", &beta_slider, beta_slider_max, on_trackbar_beta);
	on_trackbar_beta(beta_slider, 0);

	//设定饱和度初始值为50
	saturation_slider = 0;
	char satName[50];
	sprintf_s(satName, "饱和度 %d", saturation_slider_max);
	createTrackbar(satName, "Brightness and Contrastness", &saturation_slider, saturation_slider_max, HSVadjust);
	HSVadjust(saturation_slider, 0);
	

	waitKey(0);
	return 0;
}
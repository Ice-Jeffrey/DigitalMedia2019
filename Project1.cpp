#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
using namespace cv;
using std::cout;

#define CLIP_RANGE(value, min, max)  ( (value) > (max) ? (max) : (((value) < (min)) ? (min) : (value)) )
#define COLOR_RANGE(value)  CLIP_RANGE(value, 0, 255)

const int alpha_slider_max = 510;
const int beta_slider_max = 510;
const int saturation_slider_max = 255;
int alpha_slider;
int beta_slider;
int saturation_slider;
double alpha;
double beta;
double saturation;

Mat src;
Mat dst;

static void on_trackbar(int, void*) {
	/**
	Algorithm of Brightness Contrast transformation
	The formula is:
		y = [x - 127.5 * (1 - B)] * k + 127.5 * (1 + B);
		x is the input pixel value
		y is the output pixel value
		B is brightness, value range is [-1,1]
		k is used to adjust contrast
			k = tan( (45 + 44 * c) / 180 * PI );
			c is contrast, value range is [-1,1]
	*/

	double B = (beta_slider - 255) * 1.0 / 255;
	double c = (alpha_slider - 255) * 1.0 / 255;
	double k = tan((45 + 44 * c) / 180 * 3.14);
	double avg = (cv::mean(src)[0] + cv::mean(src)[1] + cv::mean(src)[2]) * 1.0 / 3;
	alpha = k;
	beta = avg * (B * k + B - k + 1);
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
			image_hsv.at<Vec3b>(i, j)[1] = COLOR_RANGE(
				image_hsv.at<Vec3b>(i, j)[1] + saturation
			);
		}
	}
	cvtColor(image_hsv, dst, CV_HSV2BGR);
	imshow("Brightness and Contrastness", dst);
}

int main(void)
{
	src = imread("C:\\Users\\Jeffrey\\Pictures\\Jerome.jpg");
	if (src.empty()) { cout << "Error loading source image! \n"; return -1; }

	namedWindow("Brightness and Contrastness", WINDOW_AUTOSIZE); // Create Window

	//设定对比度初始值为255
	alpha_slider = 255;
	char alphaName[50];
	sprintf_s(alphaName, "对比度 %d", alpha_slider_max);
	createTrackbar(alphaName, "Brightness and Contrastness", &alpha_slider, alpha_slider_max, on_trackbar);
	on_trackbar(alpha_slider, 0);

	//设定亮度初始值为255
	beta_slider = 255;
	char betaName[50];
	sprintf_s(betaName, "亮度 %d", beta_slider_max);
	createTrackbar(betaName, "Brightness and Contrastness", &beta_slider, beta_slider_max, on_trackbar);
	on_trackbar(beta_slider, 0);

	//设定饱和度初始值为50
	saturation_slider = 0;
	char satName[50];
	sprintf_s(satName, "饱和度 %d", saturation_slider_max);
	createTrackbar(satName, "Brightness and Contrastness", &saturation_slider, saturation_slider_max, HSVadjust);
	HSVadjust(saturation_slider, 0);


	waitKey(0);
	return 0;
}
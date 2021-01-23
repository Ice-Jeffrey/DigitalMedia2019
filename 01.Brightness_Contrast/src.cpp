#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using std::cout;

// 定义宏，防止像素点强度溢出
#define CLIP_RANGE(value, min, max)  ( value > max ? max : (value < min ? min : value) )
#define COLOR_RANGE(value)  CLIP_RANGE(value, 0, 255)

const int alpha_slider_max = 510;
const int beta_slider_max = 510;
const int saturation_slider_max = 200;
int alpha_slider;
int beta_slider;
int saturation_slider;
double alpha;
double beta;
double saturation;

Mat src;
Mat dst;

static void on_trackbar(int, void*) {
	/** 亮度对比度调节算法
	Algorithm of Brightness Contrast transformation
	The formula is:
		y = [x - 127.5 * (1 - B)] * k + 127.5 * (1 + B);
		x is the input pixel value
		y is the output pixel value
		B is used to adjust brightness, value range is [-1,1]
		k is used to adjust contrast:
			k = tan( (45 + 44 * c) / 180 * PI );
			c is contrast, value range is [-1,1]
	*/

	double B = beta_slider * 2.0 / 510 - 1;
	double c = alpha_slider * 2.0 / 510 - 1;
	double k = tan((45 + 44 * c) / 180 * 3.14);
	double avg = (cv::mean(src)[0] + cv::mean(src)[1] + cv::mean(src)[2]) * 1.0 / 3;
	alpha = k;
	beta = avg * (B * k + B - k + 1);
	src.convertTo(dst, -1, alpha, beta);
	
	imshow("Brightness and Contrastness", dst);
}

static void saturationAdjust(int, void*) {
	// 图像饱和度调整算法，reference: 
	// https://blog.csdn.net/xingyanxiao/article/details/48035537?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-2&spm=1001.2101.3001.4242

	saturation = saturation_slider;
	cout << src.rows << " " << src.cols << std::endl;
	
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			double H, S, L;
			unsigned char rgbMax, rgbMin;
			rgbMin = min(
				min(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i, j)[1]), 
				src.at<Vec3b>(i, j)[2]
			);
			rgbMax = max(
				max(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i, j)[1]), 
				src.at<Vec3b>(i, j)[2]
			);

			double delta = (rgbMax - rgbMin) * 1.0 / 255;
			if(delta == 0) {
				for(int k=0; k<3; k++)
					dst.at<Vec3b>(i, j)[k] = src.at<Vec3b>(i, j)[k];
				continue;
			}
			
			double value = (rgbMax + rgbMin) * 1.0 / 255;
			L = value / 2;

			if(L < 0.5)
				S = delta / value;
			else 
				S = delta / (2 - value);

			if(saturation >= 100) {
				double dPercent = (saturation - 100) / 100;
				double temp;
				if (dPercent + S >= 1) 
					temp = S;
				else 
					temp = 1 - dPercent;
				temp = 1.0 / temp - 1;

				for(int k=0; k<3; k++) {
					dst.at<Vec3b>(i, j)[k] = src.at<Vec3b>(i, j)[k] 
						+ (src.at<Vec3b>(i, j)[k] - L * 255) * temp;
				}
			}
			else {
				double temp = (saturation - 100) / 100;
				for(int k=0; k<3; k++) {
					dst.at<Vec3b>(i, j)[k] = L * 255 
						+ (src.at<Vec3b>(i, j)[k] - L * 255) * (1 + temp);
				}
			}
		}
	}
	imshow("Brightness and Contrastness", dst);
}

// 先转化为HSV，再进行饱和度调节
static void HSVadjust(int, void*) {
	Mat image_hsv;
	saturation = saturation_slider;
	cvtColor(src, image_hsv, COLOR_BGR2HSV);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//saturation_slider = image_hsv.at<Vec3b>(i, j)[1];
			image_hsv.at<Vec3b>(i, j)[1] = COLOR_RANGE(
				image_hsv.at<Vec3b>(i, j)[1] + saturation
			);
		}
	}
	cvtColor(image_hsv, dst, COLOR_HSV2BGR);
	imshow("Brightness and Contrastness", dst);
}

int main() {
	src = imread("./img/lenna.jpg");
	if (src.empty()) { cout << "Error loading source image! \n"; return -1; }

	namedWindow("Brightness and Contrastness", WINDOW_AUTOSIZE); // 新建一个窗口

	//设定对比度初始值为255
	alpha_slider = 255;
	char alphaName[50];
	sprintf(alphaName, "对比度");
	createTrackbar(alphaName, "Brightness and Contrastness", &alpha_slider, alpha_slider_max, on_trackbar);
	on_trackbar(alpha_slider, 0); 

	//设定亮度初始值为255
	beta_slider = 255;
	char betaName[50];
	sprintf(betaName, "亮度");
	createTrackbar(betaName, "Brightness and Contrastness", &beta_slider, beta_slider_max, on_trackbar);
	on_trackbar(beta_slider, 0);

	// 设定饱和度初始值为100
	saturation_slider = 100;
	char satName[50];
	sprintf(satName, "饱和度");
	createTrackbar(satName, "Brightness and Contrastness", &saturation_slider, saturation_slider_max, saturationAdjust);
	saturationAdjust(saturation_slider, 0);

	waitKey(0);
	return 0;
}
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<vector>
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


int main()
{
	//initialize the number of features to be extracted
	int NumberofFeatures = 10;

	//Create SIFT class pointer
	Ptr<SIFT> f2d = xfeatures2d::SIFT::create(NumberofFeatures);
	
	//load the image
	Mat img_1 = imread("C:/Users/Jeffrey/Pictures/test-1.jpg");
	Mat img_2 = imread("C:/Users/Jeffrey/Pictures/test-2.jpg");
	if (img_1.empty() || img_2.empty()) {
		cout << "Failed to load the image!" << endl;
		return -1;
	}

	//Detect the keypoints
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);

	//Calculate descriptors (feature vectors)
	Mat descriptors_1, descriptors_2;
	f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);

	//Matching descriptor vector using BFMatcher
	BFMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	//绘制匹配出的关键点
	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);
	imshow("【match图】", img_matches);

	//等待任意按键按下
	waitKey(0);
}
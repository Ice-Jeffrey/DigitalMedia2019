#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<vector>
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int main()
{
	//initialize the number of features to be extracted
	int NumberofFeatures = 100;

	//Create SIFT class pointer
	Ptr<SIFT> f2d = xfeatures2d::SIFT::create(NumberofFeatures);

	//load the image
	Mat img_1 = imread("C:/Users/Jeffrey/Pictures/building-left.jpg");
	Mat img_2 = imread("C:/Users/Jeffrey/Pictures/building-right.jpg");
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
	FlannBasedMatcher matcher;
	vector<vector<DMatch>> matchPoints;
	vector<DMatch> goodmatches;
	
	vector<Mat> train_desc(1, descriptors_2);

	//Add training descriptors
	matcher.add(train_desc);
	
	matcher.knnMatch(descriptors_1, matchPoints, 2);
	cout << "Total match points:" << matchPoints.size() << endl;

	//Get best matching points
	for (int i = 0; i < matchPoints.size(); i++) {
		if (matchPoints[i][0].distance < 0.4 * matchPoints[i][1].distance) {
			goodmatches.push_back(matchPoints[i][0]);
		}
	}

	drawKeypoints(img_1, keypoints_1, descriptors_1, cv::Scalar::all(-1));
	drawKeypoints(img_2, keypoints_2, descriptors_2, cv::Scalar::all(-1));

	Mat LRmatcher;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, goodmatches, LRmatcher);
	vector<Point2f> kp_l, kp_r;
	for (int i = 0; i < goodmatches.size(); i++) {
		kp_r.push_back(keypoints_2[goodmatches[i].trainIdx].pt);	//��ƥ���������
		kp_l.push_back(keypoints_1[goodmatches[i].queryIdx].pt);
	}

	//�õ��任����
	Mat homography = findHomography(kp_r, kp_l, noArray(), RANSAC);
	cout << "�任����:		\n" << homography << endl << endl;

	Mat result;		//���ս��ͼƬ

	//������ͼ����ͼ
	warpPerspective(img_2, result, homography, Size(img_1.cols + img_2.cols, img_1.rows));

	// ����ͼ���Ƶ�ǰ�沿�֣�Ȼ�����ͼ�Ž�ȥ
	Mat half(result, Rect(0, 0, img_1.cols, img_1.rows));

	//������ͼ����ͼ��ROI����
	img_1.copyTo(half);

	imshow("ƥ����", LRmatcher);
	imshow("Result", result);
	cv::imwrite("Result.jpg", result);
	waitKey(0);

	return 0;
}
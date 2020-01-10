#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
	vector<Mat>image;

	//read the images
	Mat image1, image2;
	image1 = imread("C:/Users/Jeffrey/Pictures/building-left.jpg");
	image2 = imread("C:/Users/Jeffrey/Pictures/building-right.jpg");
	image.push_back(image1);
	image.push_back(image2);


	Mat result;
	Stitcher::Mode mode = Stitcher::PANORAMA;
	Ptr<Stitcher> stitcher = Stitcher::create(mode);
	Stitcher::Status status = stitcher->stitch(image, result);

	if (status != Stitcher::OK)
	{
		cout << "Can't stitch images, error code = " << int(status) << endl;
		return EXIT_FAILURE;
	}
	namedWindow("result", 0);
	imshow("Image1", image1);
	waitKey(0);
	imshow("Image2", image2);
	waitKey(0);
	imshow("result", result);
	waitKey(0);

	return 0;
}

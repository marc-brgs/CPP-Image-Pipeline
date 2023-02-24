#ifndef OPERATIONS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define OPERATIONS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

uchar vcap(int value);

Mat brightness(Mat img, int dec);
Mat invert(Mat img);
Mat contrast(Mat img, int dec);
Mat saturate(Mat img, int dec);
Mat horizontalFlip(Mat img);
Mat verticalFlip(Mat img);
Mat rotate90(Mat img);
Mat crop(Mat img, int top, int left, int bottom, int right);
Mat add(Mat img1, Mat img2);
Mat product(Mat img1, Mat img2);

#endif
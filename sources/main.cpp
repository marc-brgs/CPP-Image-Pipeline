#include <fstream>
#include <iostream>
#include <string>
#include <array>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <windows.h>

using namespace std;
using namespace cv;

void printLine(string text);
void printSubLine(string text);
void printSubLineError(string text);

Mat brightness(Mat img, int dec);
Mat invert(Mat img);
Mat contrast(Mat img, int dec);
Mat saturate(Mat img, int dec);
Mat horizontalFlip(Mat img);
Mat verticalFlip(Mat img);
Mat rotate90(Mat img);
Mat add(Mat img1, Mat img2);
Mat product(Mat img1, Mat img2);

int vcap(int value);

int main(int argc, char** argv) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 14); // Set console color

    // Colors test
    /*for(int k = 0; k < 255; k++) {
        SetConsoleTextAttribute(hConsole, k); // Set console color
        cout << k << " test" << endl;
    }*/

    // Header
    SetConsoleTextAttribute(hConsole, 14);
    cout << "*-------------------------------*" << endl;
    cout << "|";
    SetConsoleTextAttribute(hConsole, 224);
    cout << "      Image Pipeline v0.1      ";
    SetConsoleTextAttribute(hConsole, 14);
    cout << "|" << endl;
    cout << "*-------------------------------*" << endl << endl;

    printLine("Program start");

    printLine("Loading pipeline-steps.txt");
    printSubLine("10 lines (2 images load, 8 operations)");

    // Load images
    string path = "C:\\Users/marcb/Pictures/cat-cry.jpg";
    printLine("Loading " + path);
    Mat img = imread(path, IMREAD_COLOR);

    if(img.empty()) {
        printSubLineError("[!] Error : Could not read the image");
        return 0;
    }

    string path2 = "C:\\Users/marcb/Pictures/antoine-griezmann-pp.png";
    printLine("Loading " + path2);
    Mat img2 = imread(path2, IMREAD_COLOR);

    if(img2.empty()) {
        printSubLineError("[!] Error : Could not read the image");
        //cout << "\033[0m"; // Reset console color
        return 0;
    }

    // Process images
    int step = 0;

    img = brightness(img, 40);
    step++;
    imshow("Image", img);
    waitKey(0);

    img = product(img, img2);
    step++;
    imshow("Image", img);
    waitKey(0);


    img = add(img, img2);
    step++;
    imshow("Image", img);
    waitKey(0);

    printLine("Program end");
    cout << endl;
    return 0;

    /*cout << "You have entered " << argc
         << " arguments:" << "\n";

    for (int i = 0; i < argc; ++i)
        cout << argv[i] << "\n";

    ifstream file("../pipeline-steps.txt");
    if (!file.is_open()) {
        cerr << "Impossible d'ouvrir le fichier!" << endl;
        return 1;
    }

    // Read lines
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();*/
}

void printLine(string text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 14); // Set to light yellow
    cout << "> ";
    SetConsoleTextAttribute(hConsole, 15); // Set to white
    cout << text << endl;
}

void printSubLine(string text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 14); // Set to light yellow
    cout << "\\ ";
    SetConsoleTextAttribute(hConsole, 15); // Set to white
    cout << text << endl;
}

void printSubLineError(string text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12); // Set to light red
    cout << "\\ ";
    cout << text << endl;
    SetConsoleTextAttribute(hConsole, 15); // Set to white
}

/**
 * Cap given value between 0 and 255
 */
int vcap(int value) {
    if(value > 255) return 255;
    if(value < 0) return 0;
    return value;
}

/**
 * Brightness OK
 */
Mat brightness(Mat img, int dec) {
	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & color = img.at<Vec3b>(y,x); // get pixel
			color[0] = vcap(color[0] + dec);
			color[1] = vcap(color[1] + dec);
			color[2] = vcap(color[2] + dec);
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * Invert OK
 */
Mat invert(Mat img) {
	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & color = img.at<Vec3b>(y,x); // get pixel
			color[0] = 255 - color[0];
			color[1] = 255 - color[1];
			color[2] = 255 - color[2];
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * Contrast OK
 */
Mat contrast(Mat img, int dec) {
	Mat imgHsv;
	cvtColor(img, imgHsv, CV_BGR2HSV);

	int min = 255;
	int max = 0;

	for(int y = 0; y < imgHsv.rows; y++) {
		for(int x = 0; x < imgHsv.cols; x++) {
			Vec3b & color = imgHsv.at<Vec3b>(y,x); // get pixel
			if(color[2] < min)
				min = color[2];
			else if(color[2] > max)
				max = color[2];
        }
    }

	int newMax = max + dec;
	int newMin = min - dec;

	for(int y = 0; y < imgHsv.rows; y++) {
		for(int x = 0; x < imgHsv.cols; x++) {
			Vec3b & color = imgHsv.at<Vec3b>(y,x); // get pixel
			color[2] = vcap(static_cast<int>(newMin + (newMax-newMin)*(color[2]-min)/(max-min)));
			imgHsv.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	cvtColor(imgHsv, img, CV_HSV2BGR);
	return img;
}

/**
 * Saturate (conversion to HSV make it inaccurate)
 */
Mat saturate(Mat img, int dec) {
	Mat imgHsv;
	cvtColor(img, imgHsv, CV_BGR2HSV);

	for(int y = 0; y < imgHsv.rows; y++) {
		for(int x = 0; x < imgHsv.cols; x++) {
			Vec3b & color = imgHsv.at<Vec3b>(y,x); // get pixel

            color[1] = vcap(color[1] + dec);

			imgHsv.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	cvtColor(imgHsv, img, CV_HSV2BGR);
	return img;
}

/**
 * Horizontal flip OK
 */
Mat horizontalFlip(Mat img) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			imgCopy.at<Vec3b>(Point(img.cols-1-x, y)) = img.at<Vec3b>(y,x); // get pixel
		}
	}

	return imgCopy;
}

/**
 * Vertical flip OK
 */
Mat verticalFlip(Mat img) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			imgCopy.at<Vec3b>(Point(x, img.rows-1-y)) = img.at<Vec3b>(y,x); // get pixel
		}
	}

	return imgCopy;
}

/**
 * Rotate 90° clockwise OK
 */
Mat rotate90(Mat img) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			imgCopy.at<Vec3b>(Point(img.rows-y-1, x)) = img.at<Vec3b>(y,x); // get pixel
		}
	}

	return imgCopy;
}

/**
 * Untested
 */
Mat crop(Mat img, int top, int left, int bottom, int right) {
	Mat imgCrop(right-left, bottom-top, CV_8UC3, Scalar(255, 255, 255));

	for(int y = top; y < bottom; y++) {
		for(int x = left; x < right; x++) {
			imgCrop.at<Vec3b>(Point(x-left,y-top)) = img.at<Vec3b>(y,x);
        }
    }

	return imgCrop;
}

/**
 * Untested
 */
Mat translate(Mat img, int *dec) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			imgCopy.at<Vec3b>(Point((x-dec[0])%img.cols, (y-dec[1])%img.rows)) = img.at<Vec3b>(y,x);
        }
    }

	return imgCopy;
}

/**
 * Untested
 */
Mat resize(Mat img, int *dims) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			imgCopy.at<Vec3b>(Point(x,y)) = img.at<Vec3b>((y/dims[1])*img.rows,(x/dims[0])*img.cols);
        }
    }

	return imgCopy;
}

/**
 * Untested
 */
Mat* normalize(Mat *images) {
	/*int minW = 10000, maxW = 0, minH = 10000, maxH = 0;

	for(int i = 0; i < sizeof(images); i++) {
		if(images[i].cols < minW) minW = images[i].cols;
		if(images[i].cols > maxW) maxW = images[i].cols;
		if(images[i].rows < minH) minH = images[i].rows;
		if(images[i].rows > maxH) maxH = images[i].rows;
	}

	if(minW == maxW && minH == maxH) {
		return images;
	}

	for(int i = 0; i < sizeof(images); i++) {
		images[i] = crop(images[i], 0, 0, minW, minH);
	}*/

	return images;
}

/**
 * OK
 */
Mat add(Mat img1, Mat img2) {
    Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);
			color[0] = vcap(a_color[0] + b_color[0]);
			color[1] = vcap(a_color[1] + b_color[1]);
			color[2] = vcap(a_color[2] + b_color[2]);
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * OK
 */
Mat product(Mat img1, Mat img2) {
	Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);
			color[0] = static_cast<int>(a_color[0] * b_color[0] / 255);
			color[1] = static_cast<int>(a_color[1] * b_color[1] / 255);
			color[2] = static_cast<int>(a_color[2] * b_color[2] / 255);
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * Untested
 */
Mat diff(Mat img1, Mat img2) {
	Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);
			color[0] = a_color[0] - b_color[0];
			color[1] = a_color[1] - b_color[1];
			color[2] = a_color[2] - b_color[2];
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * Untested
 */
Mat binaryMerge(Mat img1, Mat img2, Mat mask) {
	Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);
			if(mask.at<uchar>(y,x) == 0) {
				color[0] = a_color[0];
				color[1] = a_color[1];
				color[2] = a_color[2];
			}
			else {
				color[0] = b_color[0];
				color[1] = b_color[1];
				color[2] = b_color[2];
			}
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * Untested
 */
Mat weightedMerge(Mat img1, Mat img2, Mat mask) {
	Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);

			color[0] = static_cast<int>(a_color[0] * (mask.at<uchar>(y,x)/255) + b_color[0] * (1-mask.at<uchar>(y,x)/255));
			color[1] = static_cast<int>(a_color[1] * (mask.at<uchar>(y,x)/255) + b_color[1] * (1-mask.at<uchar>(y,x)/255));
			color[2] = static_cast<int>(a_color[2] * (mask.at<uchar>(y,x)/255) + b_color[2] * (1-mask.at<uchar>(y,x)/255));
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * Untested
 */
Mat screen(Mat img1, Mat img2) {
	Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);
			color[0] = static_cast<int>(255 - (255-a_color[0]) * (255-b_color[0]) /255);
			color[1] = static_cast<int>(255 - (255-a_color[1]) * (255-b_color[1]) /255);
			color[2] = static_cast<int>(255 - (255-a_color[2]) * (255-b_color[2]) /255);
			img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

	return img;
}

/**
 * ERROR
 */
/*Mat overlay(Mat img1, Mat img2) {
	Mat *images = new Mat[2];
    images[0] = img1;
    images[1] = img2;
	images = normalize(images);

	Mat a = images[0], b = images[1];
	Mat img(a.cols, a.rows, CV_8UC3, Scalar(255, 255, 255));
	Mat screen = screen(a, b);

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b & a_color = a.at<Vec3b>(y,x); // get pixel
			Vec3b & b_color = b.at<Vec3b>(y,x); // get pixel
			Vec3b & screen_color = b.at<Vec3b>(y,x); // get pixel

			Vec3b & color = img.at<Vec3b>(y,x);

			int avg = (a_color[0] + a_color[1] + a_color[2])/3;
			if(avg < 128) { // product
				color[0] = static_cast<int>(a_color[0] * b_color[0] /255);
				color[1] = static_cast<int>(a_color[0] * b_color[0] /255);
				color[2] = static_cast<int>(a_color[0] * b_color[0] /255);
				img.at<Vec3b>(Point(x,y)) = color; // set pixel
			}
			else { // screen
				img.at<Vec3b>(Point(x,y)) = screen_color; // set pixel
			}

        }
    }

	return img;
}*/

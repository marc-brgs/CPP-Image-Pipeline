#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <windows.h>

using namespace std;
using namespace cv;

void printHeader();
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
Mat crop(Mat img, int top, int left, int bottom, int right);
Mat add(Mat img1, Mat img2);
Mat product(Mat img1, Mat img2);

uchar vcap(int value);
void checkLength(vector<string> action, int size);
Mat& getImage(string name);
void showImage(string title, Mat img);


// Global variables declaration
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
map<string, Mat> stored_images;

int main(int argc, char** argv) {

    // Colors test
    /*for(int k = 0; k < 255; k++) {
        SetConsoleTextAttribute(hConsole, k); // Set console color
        cout << k << " test" << endl;
    }*/

    // Program init and locate steps file
    printHeader();

    printLine("Program start");

    printLine("Loading pipeline-steps.txt");

	ifstream file("pipeline-steps.txt");
    if (!file.is_open()) {
		printSubLineError("[!] Error : Unable to open the file");
        return 0;
    }

    // Read lines
    string line;
	vector<vector<string>> lines_comp;
	int load_count = 0;

    SetConsoleTextAttribute(hConsole, 8);
    while(getline(file, line)) {
        cout << line << endl;

        if(line == "") continue; // skip line jumps
        if(line.substr(0, 2) == "//") continue; // skip commented step

        vector<string> words_comp;
		string delimiter = " ";
		size_t pos = 0;
		string token;
		while((pos = line.find(delimiter)) != string::npos) { // split line into words_comp
			token = line.substr(0, pos);
			words_comp.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
        words_comp.push_back(line); // do not forget last token of the line

		if(words_comp.at(0) == "load") load_count++;

		lines_comp.push_back(words_comp);
    }

    file.close();

	int lines_count = static_cast<int>(lines_comp.size());
	int operation_count = lines_comp.size() - load_count;
    printSubLine(to_string(lines_count) +" lines ("+ to_string(load_count) +" load, "+ to_string(operation_count) +" operations)");

	// Processing steps

	for(int i = 0; i < lines_count; i++) {
		vector<string> action = lines_comp.at(i);
		string action_id = action.at(0);

		string line = "";
		for(int k = 0; k < action.size(); k++) {
			line += action.at(k) + " ";
		}
        stringstream sstm2;
        sstm2 << "[" << i+1 << "/" << lines_count << "] - " << line;
		printLine(sstm2.str());

        if(action_id == "load") { // ex: load image.png as img1
            checkLength(action, 4);

            string path = action.at(1);
            string image_name = action.at(3);
            printSubLine("Loading " + path);

            Mat img = imread(path, IMREAD_COLOR);
            if(img.empty()) {
                printSubLineError("[!] Error : Could not read the image");
                return 0;
            }
            stored_images.insert({image_name, img});
        }
        else if (action_id == "save") { // ex: save img1 as result.png
            checkLength(action, 4);

            string image_name = action.at(1);
            string file_name = action.at(3);
            auto it = stored_images.find(image_name);

            if (it == stored_images.end()) {
                printSubLineError("[!] Error : Image name "+ image_name +" not declared");
                return 0;
            }

            printSubLine("Saving "+ image_name +" as "+ file_name);
            imwrite(file_name, it->second);
        }
        else if(action_id == "brightness") { // ex: brightness img1 40
            checkLength(action, 3);

			string image_name = action.at(1);
			int dec = stoi(action.at(2));
			Mat& img = getImage(image_name);

			img = brightness(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
		else if(action_id == "invert") { // ex: invert img1
            checkLength(action, 2);

			string image_name = action.at(1);
			Mat& img = getImage(image_name);

			img = invert(img);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
		else if(action_id == "contrast") { // ex: contrast img1 40
            checkLength(action, 3);

			string image_name = action.at(1);
			int dec = stoi(action.at(2));
			Mat& img = getImage(image_name);

			img = contrast(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
		else if(action_id == "saturate") { // ex: saturate img1 40
			checkLength(action, 3);

			string image_name = action.at(1);
			int dec = stoi(action.at(2));

			Mat& img = getImage(image_name);
			img = saturate(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
		else if(action_id == "horizontal_flip") { // ex: horizontal_flip img1
			checkLength(action, 2);

			string image_name = action.at(1);
			Mat& img = getImage(image_name);
			img = horizontalFlip(img);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
		else if(action_id == "vertical_flip") { // ex: vertical_flip img1
			checkLength(action, 2);

			string image_name = action.at(1);

			Mat& img = getImage(image_name);
			img = verticalFlip(img);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
		else if(action_id == "rotate_90") { // ex: rotate_90 img1
			checkLength(action, 2);

			string image_name = action.at(1);

			Mat& img = getImage(image_name);
			img = rotate90(img);
            
            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + action.at(1);
            showImage(title, img);
		}
        else {
            printSubLineError("[!] Error : Unknown action in pipeline-steps.txt");
			return 0;
        }

		// Display image (at each operation on image)
		if(action_id == "load" || action_id == "save") continue;

		/*stringstream sstm3;
        sstm3 << "Step " << (i+1) << "/" << lines_count << " - " << action.at(1);
		imshow(sstm3.str(), img);
		waitKey(0);
		destroyAllWindows();*/
	}

    printLine("Program end");
    cout << endl;
    return 0;
}

/**
 * Throw an error and end program if tokens number of action is different of size
 */
void checkLength(vector<string> action, int size) {
	if(action.size() != size) {
		printSubLineError("[!] Error : "+ action.at(0) +" command should have "+ to_string(size) +" components");
		exit(0);
	}
}

/**
 * Return image object corresponding to variable name declared in steps file
 * Throw an error and end program if name unknown to program
 */
Mat& getImage(string name) {
	auto it = stored_images.find(name);
    
	if (it == stored_images.end()) {
		printSubLineError("[!] Error : Image name "+ name +" not declared");
		exit(0);
	}

	return it->second;
}

/**
 * Show image in a new window and wait user click to resume code execution
 */
void showImage(string title, Mat img) {
    namedWindow(title, CV_WINDOW_OPENGL); // fit to the screen if too big
    imshow(title, img);
    waitKey(0);
    destroyAllWindows();
}

void printHeader() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 14);
    cout << "*-------------------------------*" << endl;
    cout << "|";
    SetConsoleTextAttribute(hConsole, 224);
    cout << "      Image Pipeline v0.1      ";
    SetConsoleTextAttribute(hConsole, 14);
    cout << "|" << endl;
    cout << "*-------------------------------*" << endl << endl;
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
    cout << " \\ ";
    SetConsoleTextAttribute(hConsole, 15); // Set to white
    cout << text << endl;
}

void printSubLineError(string text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12); // Set to light red
    cout << " \\ ";
    cout << text << endl;
    SetConsoleTextAttribute(hConsole, 15); // Set to white
}

/**
 * Cap given value between 0 and 255
 */
uchar vcap(int value) {
    //if(value > 255) return 255;
    //if(value < 0) return 0;
    //return value;
	return saturate_cast<uchar>(value);
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
			imgCopy.at<Vec3b>(Point(img.cols-1-x, y)) = img.at<Vec3b>(y,x); // set pixel
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
			imgCopy.at<Vec3b>(Point(x, img.rows-1-y)) = img.at<Vec3b>(y,x); // set pixel
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
            imgCopy.at<Vec3b>(Point(img.rows-y-1, x)) = img.at<Vec3b>(y,x); // set pixel
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

			color[0] = vcap(a_color[0] * (mask.at<uchar>(y,x)/255) + b_color[0] * (1-mask.at<uchar>(y,x)/255));
			color[1] = vcap(a_color[1] * (mask.at<uchar>(y,x)/255) + b_color[1] * (1-mask.at<uchar>(y,x)/255));
			color[2] = vcap(a_color[2] * (mask.at<uchar>(y,x)/255) + b_color[2] * (1-mask.at<uchar>(y,x)/255));
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
			color[0] = vcap(255 - (255-a_color[0]) * (255-b_color[0]) /255);
			color[1] = vcap(255 - (255-a_color[1]) * (255-b_color[1]) /255);
			color[2] = vcap(255 - (255-a_color[2]) * (255-b_color[2]) /255);
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

Mat blur(Mat img, int radius) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			int sum_values[3] = {0, 0, 0};
			int num_values = 0;
			for(int i = -radius; i < radius+1; i++) {
				for(int j = -radius; j < radius+1; j++) {
					if(x+i >= 0 && x+i < img.cols && y+j >= 0 && y+j < img.rows) {
						Vec3b & color = img.at<Vec3b>(y,x);
						for(int c = 0; c < 3; c++)
							sum_values[c] += color[c];
						num_values++;
					}
				}
			}
			for(int c = 0; c < 3; c++)
				sum_values[c] /= num_values;

			Vec3b final(sum_values[0], sum_values[1], sum_values[2]);
			imgCopy.at<Vec3b>(Point(x,y)) = final; // set pixel
		}
	}

	return imgCopy;
}

Mat erode(Mat img, int radius) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b min_val(255, 255, 255);
			for(int i = -radius; i < radius+1; i++) {
				for(int j = -radius; j < radius+1; j++) {
					if(x+i >= 0 && x+i < img.cols && y+j >= 0 && y+j < img.rows) {
						Vec3b & color = img.at<Vec3b>(y,x);
						for(int c = 0; c < 3; c++)
							min_val[c] = min_val[c] < color[c] ? min_val[c] : color[c];
					}
				}
			}
			imgCopy.at<Vec3b>(Point(x,y)) = min_val; // set pixel
		}
	}

	return imgCopy;
}

Mat dilate(Mat img, int radius) {
	Mat imgCopy = img.clone();

	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			Vec3b max_val(0, 0, 0);
			for(int i = -radius; i < radius+1; i++) {
				for(int j = -radius; j < radius+1; j++) {
					if(x+i >= 0 && x+i < img.cols && y+j >= 0 && y+j < img.rows) {
						Vec3b & color = img.at<Vec3b>(y,x);
						for(int c = 0; c < 3; c++)
							max_val[c] = max_val[c] > color[c] ? max_val[c] : color[c];			    		 		   			  
					}
				}
			}
			imgCopy.at<Vec3b>(Point(x,y)) = max_val; // set pixel
		}
	}

	return imgCopy;
}

Mat rotate(Mat img, int angle, int *center) {
	Mat imgCopy = img.clone();

	float ct = cos(-angle);
	float st = sin(-angle);
	for(int y = 0; y < img.rows; y++) {
		for(int x = 0; x < img.cols; x++) {
			int a = center[0]+static_cast<int>(ct*(x-center[0])-st*(y-center[1]));
			int b = center[0]+static_cast<int>(st*(x-center[0])+ct*(y-center[1]));
			if(a >= 0 && a < img.cols && b >= 0 && b < img.rows) {
				imgCopy.at<Vec3b>(Point(x,y)) = img.at<Vec3b>(b,a); // set pixel
			}
			else {
				Vec3b zero(0, 0, 0);
				imgCopy.at<Vec3b>(Point(x,y)) = zero; // set pixel
			}
		}
	}

	return imgCopy;
}

#ifndef PIPELINE_HPP    // To make sure you don't declare the function more than once by including the header multiple times.
#define PIPELINE_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class Pipeline {
    public:
        Pipeline(){};
        int run();

    private:
        void printHeader();
        void printLine(string text);
        void printSubLine(string text);
        void printSubLineError(string text);
        void printSubLineDone(string text);
        void checkLength(vector<string> action, int size);
        Mat& getImage(string name);
        void showImage(string title, Mat img);

        map<string, Mat> stored_images;
        map<string, string> images_path;
};

#endif
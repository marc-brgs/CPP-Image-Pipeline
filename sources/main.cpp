#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "operations.h"
#include "pipeline.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Pipeline p;
    p.run();
    return 0;
}



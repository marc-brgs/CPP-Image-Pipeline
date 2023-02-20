#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    string path = "C:\\Users/marcb/Pictures/cat-cry.jpg";
    Mat img = imread(path, IMREAD_COLOR);

    if(img.empty()) {
        cout << "Could not read the image => " << path << endl;
        return 0;
    }

    for(int y=0;y<img.rows;y++) {
        for(int x=0;x<img.cols;x++) {
            Vec3b & color = img.at<Vec3b>(y,x); // get pixel
            color[0] = 13;
            color[1] = 13;
            color[2] = 13;
            img.at<Vec3b>(Point(x,y)) = color; // set pixel
        }
    }

    imshow("Image", img);

    waitKey(0);
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
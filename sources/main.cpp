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
#include "operations.h"

using namespace std;
using namespace cv;

void printHeader();
void printLine(string text);
void printSubLine(string text);
void printSubLineError(string text);

void checkLength(vector<string> action, int size);
Mat& getImage(string name);
void showImage(string title, Mat img);


// Global variables declaration
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
map<string, Mat> stored_images;
map<string, string> images_path;

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

		if(words_comp.at(0) == "load" || words_comp.at(0) == "load_mask") load_count++;

		lines_comp.push_back(words_comp);
    }

    file.close();

	int lines_count = static_cast<int>(lines_comp.size());
	int operation_count = lines_comp.size() - load_count;
    printSubLine(to_string(lines_count) +" lines ("+ to_string(load_count) +" load, "+ to_string(operation_count) +" operations)");

	// Processing steps

	for(int i = 0; i < lines_count; i++) {
		vector<string> action = lines_comp.at(i);
		string command_id = action.at(0);

		string line = "";
		for(int k = 0; k < action.size(); k++) {
			line += action.at(k) + " ";
		}
        stringstream sstm2;
        sstm2 << "[" << i+1 << "/" << lines_count << "] - " << line;
		printLine(sstm2.str());

        if(command_id == "load") { // ex: load image.png as img1
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
            images_path.insert({image_name, path});
        }
        else if(command_id == "load_mask") { // ex: load_mask mask.png as mask1
            checkLength(action, 4);

            string path = action[1];
            string image_name = action[3];
            printSubLine("Loading mask " + path);

            Mat img = imread(path, IMREAD_GRAYSCALE);
            if(img.empty()) {
                printSubLineError("[!] Error : Could not read the image");
                return 0;
            }
            stored_images.insert({image_name, img});
        }
        else if(command_id == "save") { // ex: save img1 as result.png
            checkLength(action, 4);

            string image_name = action.at(1);
            string file_name = action.at(3);
            Mat& img = getImage(image_name);

            printSubLine("Saving "+ image_name +" as "+ file_name);
            imwrite(file_name, img);
        }
        else if (command_id == "reset") { // ex: reset img1
            checkLength(action, 2);

            string image_name = action[1];
            Mat& img = getImage(image_name);

            auto it = images_path.find(image_name);

            if (it == images_path.end()) {
                printSubLineError("[!] Error : Image name "+ image_name +" not declared");
                exit(0);
            }

            img = imread(it->second, IMREAD_COLOR);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "brightness") { // ex: brightness img1 40
            checkLength(action, 3);

			string image_name = action[1];
			int dec = stoi(action[2]);
			Mat& img = getImage(image_name);

			img = brightness(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
		else if(command_id == "invert") { // ex: invert img1
            checkLength(action, 2);

			string image_name = action[1];
			Mat& img = getImage(image_name);

			img = invert(img);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
		else if(command_id == "contrast") { // ex: contrast img1 40
            checkLength(action, 3);

			string image_name = action.at(1);
			int dec = stoi(action.at(2));
			Mat& img = getImage(image_name);

			img = contrast(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
		else if(command_id == "saturate") { // ex: saturate img1 40
			checkLength(action, 3);

			string image_name = action.at(1);
			int dec = stoi(action.at(2));

			Mat& img = getImage(image_name);
			img = saturate(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
		else if(command_id == "horizontal_flip") { // ex: horizontal_flip img1
			checkLength(action, 2);

			string image_name = action.at(1);
			Mat& img = getImage(image_name);
			img = horizontalFlip(img);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
		else if(command_id == "vertical_flip") { // ex: vertical_flip img1
			checkLength(action, 2);

			string image_name = action.at(1);

			Mat& img = getImage(image_name);
			img = verticalFlip(img);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
		else if(command_id == "rotate_90") { // ex: rotate_90 img1
			checkLength(action, 2);

			string image_name = action.at(1);

			Mat& img = getImage(image_name);
			img = rotate90(img);
            
            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
		}
        else if(command_id == "crop") { // ex: crop img1 100 100 100 100
            checkLength(action, 6);

            string image_name = action[1];
            int dims[4] = {stoi(action[2]), stoi(action[3]), stoi(action[4]), stoi(action[5])}; // top left bottom right

            Mat& img = getImage(image_name);
            // force dims inside image bounds
            for(int i = 0; i < 4; i++) {
                if(dims[i] < 0)
                    dims[i] = 0;

                int extremum = img.rows-1;
                if(i%2 == 1)
                    extremum = img.cols-1;

                if(dims[i] > extremum) dims[i] = extremum;
            }

            if(dims[0] >= dims[2] || dims[1] >= dims[3]) { // check arguments coherence
                printSubLineError("[!] Error : crop command arguments top and left cannot be lower or equal than bottom and right (top left bottom right)");
                return 0;
            }

            img = crop(img, dims[0], dims[1], dims[2], dims[3]); // top left bottom right

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "translate") { // ex: translate img1 40 40
            checkLength(action, 4);
            
            string image_name = action[1];
            int *dec = new int[2];
            dec[0] = stoi(action[2]);
            dec[1] = stoi(action[3]);

            Mat& img = getImage(image_name);
            img = translate(img, dec);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "resize") { // ex: resize img1 100 100
            checkLength(action, 4);

            string image_name = action[1];
            int *dims = new int[2];
            dims[0] = stoi(action[2]);
            dims[1] = stoi(action[3]);

            Mat& img = getImage(image_name);
            img = resize(img, dims);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "add") { // ex: add img1 img2
            checkLength(action, 3);

            string image_name1 = action[1];
            string image_name2 = action[2];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            img1 = add(img1, img2);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else if(command_id == "product") { // ex: product img1 img2
            checkLength(action, 3);

            string image_name1 = action[1];
            string image_name2 = action[2];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            img1 = product(img1, img2);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else if(command_id == "diff") { // ex: diff img1 img2
            checkLength(action, 3);

            string image_name1 = action[1];
            string image_name2 = action[2];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            img1 = diff(img1, img2);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else if(command_id == "blur") { // ex: blur img1 5
            checkLength(action, 3);

            string image_name = action[1];
            int radius = stoi(action[2]);

            Mat& img = getImage(image_name);
            img = blur(img, radius);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "erode") { // ex: erode img1 5
            checkLength(action, 3);

            string image_name = action[1];
            int radius = stoi(action[2]);

            Mat& img = getImage(image_name);
            img = erode(img, radius);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "dilate") { // ex: dilate img1 5
            checkLength(action, 3);

            string image_name = action[1];
            int radius = stoi(action[2]);

            Mat& img = getImage(image_name);
            img = dilate(img, radius);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name;
            showImage(title, img);
        }
        else if(command_id == "binary_merge") { // ex: binary_merge img1 img2 mask
            checkLength(action, 4);

            string image_name1 = action[1];
            string image_name2 = action[2];
            string mask_name = action[3];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            Mat& mask = getImage(mask_name);
            img1 = binaryMerge(img1, img2, mask);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else if(command_id == "weighted_merge") { // ex: weighted_merge img1 img2 mask
            checkLength(action, 4);

            string image_name1 = action[1];
            string image_name2 = action[2];
            string mask_name = action[3];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            Mat& mask = getImage(mask_name);
            img1 = weightedMerge(img1, img2, mask);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else if(command_id == "screen") { // ex: screen img1 img2
            checkLength(action, 3);

            string image_name1 = action[1];
            string image_name2 = action[2];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            img1 = screen(img1, img2);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else if(command_id == "overlay") { // ex: overlay img1 img2
            checkLength(action, 3);

            string image_name1 = action[1];
            string image_name2 = action[2];

            Mat& img1 = getImage(image_name1);
            Mat& img2 = getImage(image_name2);
            img1 = overlay(img1, img2);

            string title = "Step " + to_string(i+1) + "/" + to_string(lines_count) + " - " + image_name1;
            showImage(title, img1);
        }
        else {
            printSubLineError("[!] Error : Unknown action in pipeline-steps.txt");
			return 0;
        }

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
    namedWindow(title, CV_WINDOW_AUTOSIZE); // fit to the screen if too big
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

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>

using namespace std;
using namespace cv;

bool exist(const string& path)
{
	ifstream f{path};
	return f.good();
}


int main()
{
	vector<Mat> imgs;

	// load files
	string basename{"imgs/"};
	string suffix{".jpg"};
	for (int i=0;; i++) {
		string filename = basename + to_string(i) + suffix;
		if (!exist(filename)) break;
		(cout << "loading " << filename << "...").flush();
		imgs.push_back(imread(filename));
		cout << "\tOK" << endl;
	}

	// stitch
	// http://docs.opencv.org/modules/stitching/doc/introduction.html
	(cout << "stitching " << imgs.size() << " images...").flush();
	Stitcher stitcher = Stitcher::createDefault(true);	// true for try_use_gpu
	Mat result;
	stitcher.stitch(imgs, result);
	cout << "\tOK" << endl;

	// save result
	(cout << "saving to result.jpg...").flush();
	imwrite("result.jpg", result);
	cout << "\tOK" << endl;
}


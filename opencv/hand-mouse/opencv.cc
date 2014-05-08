#include <err.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace std;
using namespace cv;

void text(Mat & image, const string & str, int r, int g, int b)
{
	cv::rectangle(image, Point(0, 0), Point(800, 40), Scalar(0, 0, 0), -1);
	putText(image, str, Point(10, 30), 2, 1, Scalar(b, g, r), 2);
}

void capture(VideoCapture & cap,
		function<void (Mat & image, bool is_done)> capture_cb = NULL)
{
	Mat frame;
	for (;;) {
		cap >> frame;
		if (waitKey(30) >= 0) break;
		capture_cb(frame, false);
	}
	capture_cb(frame, true);
}

int vals[6] = {0, 0, 0, 42, 255, 255};

void my_threshold(Mat & src, Mat & dst)
{
	createTrackbar("Hmin", "threshold", &vals[0], 255);
	createTrackbar("Smin", "threshold", &vals[1], 255);
	createTrackbar("Vmin", "threshold", &vals[2], 255);
	createTrackbar("Hmax", "threshold", &vals[3], 255);
	createTrackbar("Smax", "threshold", &vals[4], 255);
	createTrackbar("Vmax", "threshold", &vals[5], 255);

	GaussianBlur(src, dst, Size(11, 11), 0, 0);
	//medianBlur(src, dst, 11);
	cvtColor(dst, dst, CV_BGR2HSV);
	//inRange(dst, Scalar(0, 120, 140), Scalar(10, 255, 255), dst);
	inRange(dst, Scalar(vals[0], vals[1], vals[2]), Scalar(vals[3], vals[4], vals[5]), dst);
	morphologyEx(dst, dst, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(9, 9), Point(4, 4)));
	GaussianBlur(dst, dst, Size(3, 3), 0, 0);
}

int rmp(int src, float sf, float st, float df, float dt)
{
	if (src < sf) src = sf;
	if (src > st) src = st;
	return (src-sf) / (st-sf) * (dt-df) + df;
}

int main(int argc, char * argv[])
{
	VideoCapture cap(0);
	if (!cap.isOpened()) err(1, "unable to open capture device.");

	//namedWindow("camera");
	//moveWindow("camera", 0, 0);
	namedWindow("threshold");
	moveWindow("threshold", 720, 0);

	Mat image_thresh;
	vector<Point2i> ps;
	capture(cap, [&](Mat & image, bool is_done) {
		if (is_done) {
			destroyWindow("camera");
		}
		else {
			cv::flip(image, image, 1);
			//imshow("camera", image);
			my_threshold(image, image_thresh);

			Moments mms = moments(image_thresh);
			double m10 = mms.m10;
			double m01 = mms.m01;
			double area = mms.m00;
			Point2i p(m10/area, m01/area);

			int x = rmp(p.x, 200, image.cols-200, 0, 1366);
			int y = rmp(p.y, 200, image.rows-200, 0,  768);
			{
				stringstream ss;
				ss << "xdotool mousemove " << x << " " << y;
				string s = ss.str();
				//cout << s << endl;
				system(s.c_str());
			}

			cvtColor(image_thresh, image_thresh, CV_GRAY2BGR);

			if (p.x > 0) ps.push_back(p);
			if (ps.size() > 100) ps.erase(ps.begin());
			if (ps.size() > 2)
				for (int i=0; i<(int)ps.size()-1; i++) {
					float a = 1.0f - (float)i / ps.size();
					cv::line(image, ps[i], ps[i+1], Scalar(255*a, 255*a, 255), 3);
					cv::line(image_thresh, ps[i], ps[i+1], Scalar(255*a, 255*a, 255), 3);
				}

			stringstream ss;
			ss << p ;
			text(image, ss.str(), 0, 255, 100);

			//cvtColor(image_thresh, image_thresh, CV_GRAY2BGR);
			//multiply(image, image_thresh, image);
			imshow("threshold", image_thresh);
		}
	});

	return 0;
}


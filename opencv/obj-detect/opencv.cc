#include <err.h>
#include <iostream>
#include <opencv2/opencv.hpp>

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

int vals[6] = {130, 0, 0, 180, 255, 255};

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

void draw_poly(Mat & image, vector<Point> & poly, int r, int g, int b)
{
	if (poly.size() < 2) return;
	for (int i=0; i<poly.size()-1; i++)
		cv::line(image, poly[i], poly[i+1], Scalar(b, g, r), 2);
	cv::line(image, poly[poly.size()-1], poly[0], Scalar(b, g, r), 2);
}

int main(int argc, char * argv[])
{
	VideoCapture cap(0);
	if (!cap.isOpened()) err(1, "unable to open capture device.");

	namedWindow("camera");
	moveWindow("camera", 0, 0);
	namedWindow("threshold");
	moveWindow("threshold", 720, 0);

	Mat image_thresh;
	vector<Point2i> ps;
	capture(cap, [&](Mat & image, bool is_done) {
		if (is_done) {
			destroyWindow("camera");
		}
		else {
			imshow("camera", image);
			my_threshold(image, image_thresh);

			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			Canny(image_thresh, image, 100, 200, 3);
			findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			Mat image_contours = Mat::zeros(image.size(), CV_8UC3);
			for (int i=0; i<contours.size(); i++)
				drawContours(image_contours, contours, i, Scalar(255, 0, 0));

			// find the greatest area contour
			auto max_contour = contours[0];
			float max_contour_area = 0;
			for (auto contour : contours) {
				float area = fabs(contourArea(contour));
				if (area > max_contour_area) {
					max_contour_area = area;
					max_contour = contour;
				}
			}
			approxPolyDP(max_contour, max_contour, 2, true);
			draw_poly(image_contours, max_contour, 255, 0, 0);

			imshow("contours", image_contours);

			Moments mms = moments(image_thresh);
			double m10 = mms.m10;
			double m01 = mms.m01;
			double area = mms.m00;
			Point2i p(m10/area, m01/area);

			cvtColor(image_thresh, image_thresh, CV_GRAY2BGR);

			if (p.x > 0) ps.push_back(p);
			if (ps.size() > 100) ps.erase(ps.begin());
			if (ps.size() > 2)
				for (int i=0; i<ps.size()-1; i++) {
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


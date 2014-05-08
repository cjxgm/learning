#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using Contour = vector<Point>;
using Contours = vector<Contour>;
using Hierarchy = vector<Vec4i>;
RNG rng(1995);

void sobel_edge(Mat& src, Mat& dst)
{
	Mat grad_x, grad_y;

	Sobel(src, grad_x, CV_16S, 1, 0);
	convertScaleAbs(grad_x, grad_x);

	Sobel(src, grad_y, CV_16S, 0, 1);
	convertScaleAbs(grad_y, grad_y);

	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst);
}


int main()
{
	VideoCapture cap{0};
	if (!cap.isOpened())
		throw runtime_error{"unable to open capture device."};

	namedWindow("edge");
	moveWindow("edge", 720, 0);

	namedWindow("contour");
	moveWindow("contour", 0, 400);

	for (;;) {
		constexpr int fps = 30;
		if (waitKey(1000/fps) >= 0) break;




		// read in frame
		Mat frame;
		cap >> frame;
		cv::flip(frame, frame, 1);
		imshow("camera", frame);




		// detect edge
		Mat edge;

		static int k=11, x=10;
		createTrackbar("k", "edge", &k, 11);
		createTrackbar("x", "edge", &x, 10);
		if (!(k&1)) k++;
		GaussianBlur(frame, edge, Size(k, k), x, x);

		cvtColor(edge, edge, CV_BGR2GRAY);
		equalizeHist(edge, edge);
		imshow("edge", edge);

		static int a=230;
		createTrackbar("a", "edge", &a, 255);
		//Canny(edge, edge, a, b, 3, true);
		sobel_edge(edge, edge);
		//inRange(edge, Scalar(a, a, a), Scalar(b, b, b), edge);
		equalizeHist(edge, edge);
		threshold(edge, edge, a, 255, CV_THRESH_BINARY);

		//imshow("edge", edge);




		// find coutour
		Contours cons, hulls;
		Hierarchy hie;

		findContours(edge, cons, hie, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
		for (auto con: cons) {
			Contour hull;
			convexHull(con, hull);
			hulls.push_back(hull);
		}

		cvtColor(edge, edge, CV_GRAY2BGR);
		double max_area = 0;
		unsigned max_i = 0;
		for (unsigned i=0; i<cons.size(); i++) {
			double area = contourArea(hulls[i]);
			if (area > max_area) {
				max_area = area;
				max_i = i;
			}
		}
		Contour& con = cons[max_i];
		Contour& hull = hulls[max_i];
		approxPolyDP(con, con, 0.01, true);
		convexHull(con, hull);
		drawContours(edge, cons, max_i, Scalar(255, 255, 255), 2, 8, hie);
		drawContours(edge, hulls, max_i, Scalar(255, 255, 255), 2, 8, hie);

		imshow("contour", edge);
	}
}


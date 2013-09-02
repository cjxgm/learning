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
		cvtColor(frame, frame, CV_BGR2HSV);
		extractChannel(frame, frame, 0);
		blur(frame, frame, Size(2, 2));
		cvtColor(frame, frame, CV_GRAY2BGR);
		if (waitKey(30) >= 0) break;
		capture_cb(frame, false);
	}
	capture_cb(frame, true);
}

int main(int argc, char * argv[])
{
	VideoCapture cap(0);
	if (!cap.isOpened()) err(1, "unable to open capture device.");

	Mat image1, image2;
	Mat desc1, desc2;
	vector<KeyPoint> kps1, kps2;
	OrbFeatureDetector detector(400);
	OrbDescriptorExtractor extractor;

	capture(cap, [&](Mat & image, bool is_done) {
		if (is_done) {
			cvtColor(image, image1, CV_BGR2GRAY);
			destroyWindow("train");
		}
		else {
			text(image, "Press any key to comfirm...", 255, 100, 100);
			imshow("train", image);
		}
	});

	detector.detect(image1, kps1);

	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	Mat image_matches;
	capture(cap, [&](Mat & image, bool is_done) {
		cvtColor(image, image2, CV_BGR2GRAY);
		detector.detect(image2, kps2);

		extractor.compute(image1, kps1, desc1);
		extractor.compute(image2, kps2, desc2);
		desc1.convertTo(desc1, CV_32F);
		desc2.convertTo(desc2, CV_32F);
		matcher.match(desc1, desc2, matches);

		// find good matches
		double max_dist = 0, min_dist = 100;
		for (auto match : matches) {
			double dist = match.distance;
			if (dist < min_dist) min_dist = dist;
			if (dist > min_dist) max_dist = dist;
		}
		vector<DMatch> good_matches;
		for (auto match : matches)
			if (match.distance <= 2*min_dist)
				good_matches.push_back(match);

		// draw good matches
		drawMatches(image1, kps1, image2, kps2, good_matches, image_matches);
		stringstream ss;
		ss << "[min, max] = [" << min_dist << ", " << max_dist << "]";
		text(image_matches, ss.str(), 0, 100, 255);

		// homography object detection
		vector<Point2f> ps1, ps2;
		for (auto match : good_matches) {
			ps1.push_back(kps1[match.queryIdx].pt);
			ps2.push_back(kps2[match.trainIdx].pt);
		}
		if (ps1.size() >= 4) {
			Mat homo = findHomography(ps1, ps2);
			vector<Point2f> corners1(4), corners2(4);
			corners1[0] = cvPoint(0, 0);
			corners1[1] = cvPoint(image1.cols, 0);
			corners1[2] = cvPoint(image1.cols, image1.rows);
			corners1[3] = cvPoint(0, image1.rows);
			perspectiveTransform(corners1, corners2, homo);
			Point2f offset(image1.cols, 0);
			cv::line(image_matches, corners2[0] + offset, corners2[1] + offset, Scalar(50, 255, 50));
			cv::line(image_matches, corners2[1] + offset, corners2[2] + offset, Scalar(50, 255, 50));
			cv::line(image_matches, corners2[2] + offset, corners2[3] + offset, Scalar(50, 255, 50));
			cv::line(image_matches, corners2[3] + offset, corners2[0] + offset, Scalar(50, 255, 50));
			stringstream ss;
			ss << "H: " << corners2;
			text(image_matches, ss.str(), 0, 100, 255);
		}

		imshow("matches", image_matches);
	});

	return 0;
}


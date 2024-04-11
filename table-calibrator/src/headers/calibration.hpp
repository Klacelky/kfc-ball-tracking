#pragma once
#include <opencv2/videoio.hpp>

struct  MouseData {
	int x = 0;
	int y = 0;
	bool clicked = false;
};

void start_calibration(cv::VideoCapture video);

void mouse_controller(int event, int x, int y, int flags, void* userdata);

void draw_points(cv::Mat frame, std::vector<cv::Point2f> points);

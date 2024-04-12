#pragma once
#include <opencv2/videoio.hpp>

struct  MouseData {
	cv::Point2f pos;
	bool clicked = false;
};

void start_calibration(cv::VideoCapture video);

void mouse_controller(int event, int x, int y, int flags, void* userdata);

void draw_points(cv::Mat& frame, cv::Point2f points[], int count);

void draw_help(cv::Mat& frame, const char* str);

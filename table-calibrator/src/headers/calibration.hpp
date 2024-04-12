#pragma once
#include <opencv2/videoio.hpp>

struct  MouseData {
	cv::Point2f pos;
	bool clicked = false;
};

void start_calibration(cv::VideoCapture video);

void mouse_controller(int event, int x, int y, int flags, void* userdata);

void get_frame(cv::VideoCapture& video, cv::Mat& frame);

void draw_help(cv::Mat& frame, const char* str);

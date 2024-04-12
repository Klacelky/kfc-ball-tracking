#pragma once
#include <opencv2/highgui.hpp>

void tracking_loop(cv::VideoCapture& video);

bool find_ball(cv::Mat& frame, cv::Point2f& out_pos);

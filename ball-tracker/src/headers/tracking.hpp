#pragma once
#include <opencv2/highgui.hpp>
#include "calibdata.hpp"

void tracking_loop(cv::VideoCapture& video);

bool find_ball(cv::Mat& frame, cv::Point2f& out_pos);

cv::Point2f to_table_space(const cv::Point2f& p_ss, const CalibData& cdata);

bool in_table(const cv::Point2f& p, const CalibData& cdata);

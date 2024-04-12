#pragma once
#include <opencv2/core/types.hpp>

struct CalibData 
{
	float table_width  = 0.0f;
	float table_height = 0.0f;
	cv::Point2f src[4];
	cv::Point2f dst[4];

	int save(const char* path) const;

	int load(const char* path);
};


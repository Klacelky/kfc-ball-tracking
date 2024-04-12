#pragma once
#include <cstdint>
#include <opencv2/core/mat.hpp>

#define CALIB_DATA_FILE_PATH "calib-data"

struct CalibData {
	const uint32_t table_width;
	const uint32_t table_height;
	const cv::Mat persp_mat;

	int save() const;

	int load(CalibData* out);
};


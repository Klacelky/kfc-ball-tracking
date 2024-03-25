#pragma once
#include <opencv2/highgui.hpp>

using namespace cv;

inline void CropFrame(Mat* cropped, Mat& frame, int left, int right, int top, int bot);

inline void DownsampleFrame(Mat* downsampled, Mat& frame, int factor);

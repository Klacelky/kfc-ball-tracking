#pragma once
#include <opencv2/videoio.hpp>

// Track return codes
#define EOF_REACHED 0
#define USER_INTERRUPT 1
#define ERROR_INTERRUPT 2 

using namespace cv;

int Track(VideoCapture &video);

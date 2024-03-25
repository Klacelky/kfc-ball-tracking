#pragma once
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

// Choose what should be visible in debug
#ifdef _DEBUG
	#define TR_SHOW_FRAMES
#endif

// Track return codes
#define EOF_REACHED 0
#define USER_INTERRUPT 1
#define ERROR_INTERRUPT 2 

using namespace cv;

int Track(VideoCapture &video);

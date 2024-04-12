#pragma once
#include <iostream>

#ifdef _DEBUG
#define DEBUG_WAIT(time)							cv::waitKey(time)
#define DEBUG_COUT(msg)								std::cout << msg << std::endl
#define DEBUG_SHOW(name, frame)						cv::imshow(name, frame)
#define DEBUG_POINT(frame, center, radius, color)	cv::circle(frame,center, radius, color, cv::FILLED)
#else
#define DEBUG_WAIT(time)							
#define DEBUG_COUT(msg)								
#define DEBUG_SHOW(name, frame)						
#define DEBUG_POINT(frame, center, radius, color)	
#endif



#include "tracking.hpp"

using namespace cv;

int Track(VideoCapture &video)
{
	Mat frame;
	const unsigned int WAIT_TIME = 30;

	while (video.read(frame))
	{
		#ifdef TR_SHOW_FRAMES
		imshow("frame", frame);
		if ('q' == waitKey(WAIT_TIME)) {
			return USER_INTERRUPT;
		}
		#endif 
	}
	return EOF_REACHED;
}
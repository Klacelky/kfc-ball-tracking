#include "tracking.hpp"
#include "frames.hpp"

using namespace cv;

int Track(VideoCapture &video)
{
	const unsigned int WAIT_TIME = 30;
	const Scalar BALL_COLOR_LB(40, 200, 200); // BGR!
	const Scalar BALL_COLOR_UP(80, 255, 255);

	Mat frame, cropped, downsmp, mask;

	while (video.read(frame))
	{
		#ifdef TR_SHOW_FRAMES
		imshow("frame", frame);
		#endif
		CropFrame(&cropped, frame, 450, 450, 0, 0);
		DownsampleFrame(&downsmp, cropped, 2);
		FrameColorMask(&mask, downsmp, BALL_COLOR_LB, BALL_COLOR_UP);
		if ('q' == waitKey(WAIT_TIME)) {
			return USER_INTERRUPT;
		}

	}
	return EOF_REACHED;
}
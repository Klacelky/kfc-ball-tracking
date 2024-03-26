#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

void CropFrame(Mat* cropped, Mat& frame, int left, int right, int top, int bot)
{
	Rect roi(left, top, frame.cols - left - right, frame.rows - bot - top);
	*cropped = Mat(frame, roi);
	#ifdef TR_SHOW_CROPPED
	imshow("Cropped", *cropped);
	#endif
}

void DownsampleFrame(Mat* downsampled, Mat& frame, int factor)
{
	resize(frame, *downsampled, Size(), 1.0 / factor,  1.0 / factor, INTER_NEAREST);
	#ifdef TR_SHOW_DOWNSAMPLED
	imshow("Downsampled", *downsampled);
	#endif
}

void FrameColorMask(Mat* mask, Mat& frame, Scalar lowerb, Scalar upperb)
{
	inRange(frame, lowerb, upperb, *mask);
	#ifdef TR_SHOW_MASK
	imshow("Mask", *mask);
	#endif
}

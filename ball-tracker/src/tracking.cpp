#pragma comment(lib, "ws2_32.lib")

#include "headers/debug.hpp"
#include "headers/settings.hpp"
#include "headers/tracking.hpp"
#include "calibdata.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <winsock2.h>
#include <Ws2tcpip.h>

void tracking_loop(cv::VideoCapture& video)
{
    cv::Mat frame;
	cv::Point2f pos_ss, pos_ts;
	BallPos last1, last2;
	CalibData cdata;

	bool counter_paused = false;
	uint counter = 0;
	uint frame_index = 0;

	const double FPS = video.get(cv::CAP_PROP_FPS);

	if (!cdata.load("calib-data")) {
		return;
	}

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addrlisten = {};
	addrlisten.sin_family = AF_INET;
	if (bind(s, (sockaddr*)&addrlisten, sizeof(addrlisten)) == -1) {
		perror("tracking_loop:bind");
		return;
	}
	sockaddr_in addrTarget = {};
	addrTarget.sin_family = AF_INET;
	if (!inet_pton(AF_INET, IP, &addrTarget.sin_addr.S_un.S_addr)) {	// IP
		perror("tracking_loop:inet_pton");
		return;
	}
	addrTarget.sin_port = htons(PORT);				// port

    while (true) {
        if (!video.read(frame)) {
            continue;
        }
		++frame_index;

		#ifdef _DEBUG
		cv::Mat framecpy = frame.clone();
		static const cv::Scalar RED(0, 0, 255);
		DEBUG_WAIT(5);
		DEBUG_COUT("frame " << frame_index << ":");
		#endif 
		
		if (find_ball(frame, pos_ss)) 
		{	
			pos_ts = to_table_space(pos_ss, cdata);
			if (!in_table(pos_ts, cdata)){
				DEBUG_COUT(" " << last1.pos_ss << " outside table" << std::endl);
				DEBUG_SHOW("Frame", framecpy);
				continue;
			}
			last2 = last1;
			last1 = { pos_ss, pos_ts, frame_index };
			counter = 0;
			counter_paused = false;

			DEBUG_COUT(" " << last1.pos_ss << std::endl);
			DEBUG_POINT(framecpy, pos_ss, 5, RED);
			DEBUG_SHOW("Frame", framecpy);
			continue;
		}

		DEBUG_COUT(" not found");
		if (counter_paused) 
		{
			DEBUG_COUT(" (paused)" << std::endl);
			DEBUG_LINE(framecpy, last2.pos_ss, last1.pos_ss, RED);
			DEBUG_SHOW("Frame", framecpy);
			continue;
		}

		++counter;
		DEBUG_COUT(" (" << counter << ")" << std::endl);

		if (counter_paused = counter >= GOAL_FRAME_COUNT) 
		{
			auto df = last1 - last2;
			auto dist = sqrt(df.pos_ts.x * df.pos_ts.x + df.pos_ts.y * df.pos_ts.y);
			auto time = df.frame / FPS;
			auto speed = dist / time;	// in [table space unit] per second

			Shot shot = { last2.pos_ss.x, last2.pos_ss.y, last1.pos_ss.x, last1.pos_ss.y, (uint32_t) speed };
			sendto(s, (const char*)&shot, sizeof(Shot), 0, (sockaddr*)&addrTarget, sizeof(addrTarget));
			
			DEBUG_COUT("\ngoal with speed : " << (int)speed << std::endl);
			DEBUG_LINE(framecpy, last2.pos_ss, last1.pos_ss, RED);
			DEBUG_SHOW("Frame", framecpy);
		}
	}
}

		/*if (!find_ball(frame, pos_ss)) {
			if (counter_paused) {
				DEBUG_LINE(framecpy, last2.pos_ss, last1.pos_ss, RED);
				DEBUG_COUT("not found (paused)");
			}
			else {
				DEBUG_COUT("not found (" << counter + 1 << ")");
				if (++counter >= GOAL_FRAME_COUNT) {
					counter_paused = true;
					BallPos df = last1 - last2;
					auto dist = sqrt(df.pos_ts.x * df.pos_ts.x + df.pos_ts.y * df.pos_ts.x);
					double speed = dist / (df.frame / FPS);
					
					DEBUG_COUT("\ngoal: " << speed);
					DEBUG_LINE(framecpy, last2.pos_ss, last1.pos_ss, RED);
				}
			}
		} else {
			pos_ts = to_table_space(pos_ss, cdata);
			if (in_table(pos_ts, cdata)) {
				counter_paused = false;
				counter = 0;
				last2 = last1;
				last1 = { pos_ss, pos_ts, frame_index };
				DEBUG_COUT(pos_ts << " " << frame_index);
				DEBUG_POINT(framecpy, pos_ss, 5, RED);
			}
			else {
				DEBUG_COUT("outside table");
			}
		}
		DEBUG_WAIT(5);
        DEBUG_SHOW("Frame", framecpy);
    }
}*/

bool find_ball(cv::Mat& frame, cv::Point2f& out_pos)
{
	float radius;
	std::vector<cv::Point> ball_pixels;

	#ifdef CROP
	static const cv::Point2f top_left(CROP_TOP_LEFT_X, CROP_TOP_LEFT_Y);
	static const cv::Point2f bot_right(CROP_BOT_RIGHT_X, CROP_BOT_RIGHT_Y);
	static const cv::Rect roi(top_left, bot_right);
	frame = cv::Mat(frame, roi);
	DEBUG_SHOW("Crop", frame);
	#endif

	#ifdef DOWNSAMPLE
	static const cv::Size size = cv::Size();
	resize(frame, frame, size, DOWNSAMPLE_FACTOR, DOWNSAMPLE_FACTOR, cv::INTER_NEAREST);
	DEBUG_SHOW("Downsample", frame);
	#endif

	#ifdef BLURR
	static const cv::Size blurr_size(BLURR_RADIUS, BLURR_RADIUS);
	GaussianBlur(frame, frame, blurr_size, BLURR_SIGMA);
	DEBUG_SHOW("BLurr", frame);
	#endif

	// create mask
	static const cv::Scalar ball_color_min(BALL_BLUE_MIN, BALL_GREEN_MIN, BALL_RED_MIN);
	static const cv::Scalar ball_color_max(BALL_BLUE_MAX, BALL_GREEN_MAX, BALL_RED_MAX);
	inRange(frame, ball_color_min, ball_color_max, frame);
	DEBUG_SHOW("Mask", frame);

	#ifdef MORPHOLOGY
	static const cv::Size morph_size(MORPH_RADIUS, MORPH_RADIUS);
	static const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, morph_size);
	morphologyEx(frame, frame, MORPH_OPERATION, kernel);
	DEBUG_SHOW("Morphology", frame);
	#endif

	#ifdef _DEBUG
	static const cv::Scalar RED(0, 0, 255);
	static const cv::Scalar GREEN(0, 255, 0);
	cv::Mat enclosure = cv::Mat::zeros(frame.size(), CV_8UC3);
	#endif

	// collect ball pixels
	cv::findNonZero(frame, ball_pixels);
	if (ball_pixels.empty()) {
		return false;
	}

	// find min enclosing circle
	cv::minEnclosingCircle(ball_pixels, out_pos, radius);

	#ifdef CHECK_RADIUS
	if (radius < BALL_RADIUS_MIN * DOWNSAMPLE_FACTOR || radius > BALL_RADIUS_MAX * DOWNSAMPLE_FACTOR) {
		DEBUG_POINT(enclosure, out_pos, radius, RED);
		DEBUG_SHOW("Circle enclosure", enclosure);
		return false;
	}
	#endif

	#ifdef CHECK_COVERAGE
	double coverage = ball_pixels.size() / (2 * 3.14 * radius);
	if (coverage < BALL_AREA_COVERAGE_MIN) {
		DEBUG_POINT(enclosure, out_pos, radius, RED);
		DEBUG_SHOW("Circle enclosure", enclosure);
		return false;
	}
	#endif

	DEBUG_POINT(enclosure, out_pos, radius, GREEN);
	DEBUG_SHOW("Circle enclosure", enclosure);

	// convert position to original frame
	#ifdef DOWNSAMPLE
	out_pos = out_pos * 1.0 / DOWNSAMPLE_FACTOR;
	#endif
	#ifdef CROP
	out_pos.x += CROP_TOP_LEFT_X;
	out_pos.y += CROP_TOP_LEFT_Y;
	#endif 

	return true;
}

cv::Point2f to_table_space(const cv::Point2f& p_ss, const CalibData& cdata)
{
	static const cv::Mat persp_mat = cv::getPerspectiveTransform(cdata.src, cdata.dst);
	cv::Mat p_ts(persp_mat * (cv::Mat_<double>(3, 1) << p_ss.x, p_ss.y, 1.0));
	double w = p_ts.at<double>(2);
	return { (float) (p_ts.at<double>(0) / w), (float) (p_ts.at<double>(1) / w)};
}

inline
bool in_table(const cv::Point2f& p, const CalibData& cdata)
{
	return p.x >= 0 && p.y >= 0 && p.x <= cdata.table_width && p.y <= cdata.table_height;
}

BallPos BallPos::operator-(const BallPos& other) const
{
	return { pos_ss - other.pos_ss,
			 pos_ts - other.pos_ts,
			 (frame > other.frame) ? frame - other.frame : other.frame - frame };
}
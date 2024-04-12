#pragma once

// pipline

#define CROP
#define DOWNSAMPLE
//#define BLURR
//#define MORPHOLOGY

//#define CHECK_RADIUS
//#define CHECK_COVERAGE

// pipline settings

#define CROP_TOP_LEFT_X  450
#define CROP_TOP_LEFT_Y	 50
#define CROP_BOT_RIGHT_X 1415
#define CROP_BOT_RIGHT_Y 1010

#define DOWNSAMPLE_FACTOR 0.5

#define BLURR_RADIUS 3
#define BLURR_SIGMA	 5

#define MORPH_RADIUS 5
#define MORPH_OPERATION cv::MORPH_CLOSE

#define BALL_AREA_COVERAGE_MIN 0.25

// ball

#define BALL_RED_MIN	200
#define BALL_GREEN_MIN	200
#define BALL_BLUE_MIN	0

#define BALL_RED_MAX	255
#define BALL_GREEN_MAX	255
#define BALL_BLUE_MAX	80

#define BALL_RADIUS_MIN 10
#define BALL_RADIUS_MAX 20

// goal settings

#define GOAL_FRAME_COUNT 25
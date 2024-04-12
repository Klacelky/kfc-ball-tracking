#include "headers/calibration.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define WINDOW_NAME         "Calibration"
#define HELP_TOP_LEFT       "[LMB] Select top left "
#define HELP_TOP_RIGHT      "[LMB] Select top right "
#define HELP_BOT_LEFT       "[LMB] Select bottom left "
#define HELP_BOT_RIGHT      "[LMB] Select bottom right "
#define HELP_REST_SELECT    "[R] Restart selection "
#define HELP_SAVE_QUIT      "[S] Save and quit "
#define HELP_QUIT           "[Q] Quit "
#define HELP_CONTINUE       "[ENTER] Continue "
#define HELP_CONSOLE_INPUT  "[CONSOLE] Enter table dimensions "

void start_calibration(cv::VideoCapture video)
{
    const char* const HELP[6] = { HELP_REST_SELECT HELP_TOP_LEFT, 
                                  HELP_REST_SELECT HELP_TOP_RIGHT,
                                  HELP_REST_SELECT HELP_BOT_LEFT, 
                                  HELP_REST_SELECT HELP_BOT_RIGHT,
                                  HELP_REST_SELECT HELP_CONTINUE,
                                  HELP_REST_SELECT HELP_SAVE_QUIT HELP_QUIT};

    MouseData mdata;
    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, mouse_controller, &mdata);

    cv::Mat frame;
    cv::Point2f src[4];
    char selected = 0;
selection_start:
    selected = 0;
    mdata.clicked = false;
    while (true)
    {

        if (!video.read(frame)) {
            continue;
        }

        if (mdata.clicked && selected < 4) {
            src[selected++] = mdata.pos;
            mdata.clicked = false;
        }

        draw_points(frame, src, selected);
        draw_help(frame, HELP[selected]);
        cv::imshow(WINDOW_NAME, frame);

        int key = cv::waitKey(10);
        if ('r' == key) {
            goto selection_start;
        }
        if (4 == selected && 13 == key) {   // enter
            break;
        }
    }

    float width, height;
    std::cout << "Enter table width in milimeters: ";   
    std::cin  >> width;
    std::cout << "Enter table height in milimeters: ";  
    std::cin  >> height;
    cv::Point2f dst[4] = { {0,0}, {width, 0}, {0, height}, {width, height} };
    const cv::Mat PERSP_MAT = cv::getPerspectiveTransform(src, dst);

    cv::Mat wframe;
    while (true)
    {
        if (!video.read(frame)) {
            continue;
        }
        cv::warpPerspective(frame, wframe, PERSP_MAT, cv::Size(width, height), cv::INTER_NEAREST);
        cv::resize(wframe, wframe, cv::Size(frame.rows * width / height, frame.rows), cv::INTER_NEAREST);
        draw_help(wframe, HELP[5]);
        cv::imshow(WINDOW_NAME, wframe);
        int key = cv::waitKey(10);
        if ('r' == key) {
            goto selection_start;
        }
        if ('s' == key) {
            continue;
        }
        if ('q' == key) {
            return;
        }
    }
}

void mouse_controller(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN) {
        MouseData* mdata = static_cast<MouseData*>(userdata);
        mdata->pos.x = x;
        mdata->pos.y = y;
        mdata->clicked = true;
    }
}

void draw_points(cv::Mat& frame, cv::Point2f points[], int count)
{
    static const cv::Scalar RED(0, 0, 255);
    for (int i = 0; i < count; i++) {
        cv::circle(frame, points[i], 5, RED , cv::FILLED);
    }
}

void draw_help(cv::Mat &frame, const char* str)
{
    static const cv::Rect bg(0, 0, 500, 40);
    static const cv::Scalar WHITE(0, 0, 0);
    static const cv::Scalar BLACK(255, 255, 245);
    cv::rectangle(frame, bg, WHITE, cv::FILLED);
    cv::putText(frame, cv::String(str), cv::Point(5, 25), cv::FONT_HERSHEY_SIMPLEX, 0.6, BLACK);
}

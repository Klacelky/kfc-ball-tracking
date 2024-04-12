#include "headers/calibration.hpp"
#include "headers/calibdata.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define WINDOW_NAME     "Calibration"
#define SAVE_FILE_PATH  "calib-data"
#define HELP_START      "[L] load  [ENTER] start calibration"
#define HELP_RESULT     "[R] restart selection  [S] save  [Q] quit"
#define HELP_SELECTION1 "[R] restart selection  [LMB] select TOP LEFT"
#define HELP_SELECTION2 "[R] restart selection  [LMB] select TOP RIGHT"
#define HELP_SELECTION3 "[R] restart selection  [LMB] select BOTTOM LEFT"
#define HELP_SELECTION4 "[R] restart selection  [LMB] select BOTTOM RIGHT"
#define HELP_CIN        "Enter input into console!"

void start_calibration(cv::VideoCapture video)
{
    const char* const HELP_SELECT[4] = { HELP_SELECTION1, HELP_SELECTION2, HELP_SELECTION3, HELP_SELECTION4 };
    const cv::Scalar RED(0, 0, 255);

    cv::Mat frame;
    cv::Mat wframe;
    cv::Mat persp_mat;
    MouseData mdata;
    CalibData cdata;
    char selected;

    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, mouse_controller, &mdata);

main_selection:
    while (true) {
        get_frame(video, frame);
        draw_help(frame, HELP_START);
        cv::imshow(WINDOW_NAME, frame);
        int key = cv::waitKey(10);
        if ('l' == key) {
            cdata.load(SAVE_FILE_PATH);
            goto show_warp;
        }
        if (13 == key) {   // enter
            break;
        }
    }

point_selection:
    mdata.clicked = false;
    for (int i = 0; i < 4; i++) {
        draw_help(frame, HELP_SELECT[i]);
        cv::imshow(WINDOW_NAME, frame);
        while (!mdata.clicked) {
            if ('r' == cv::waitKey(10)) {
                goto main_selection;
            }
        }
        cdata.src[i] = mdata.pos;
        mdata.clicked = false;
        cv::circle(frame, mdata.pos, 5, RED, cv::FILLED);
        cv::imshow(WINDOW_NAME, frame);
    }

    draw_help(frame, HELP_CIN);
    cv:imshow(WINDOW_NAME, frame);
    cv::waitKey(1);
    std::cout << "Enter table width in milimeters: ";
    std::cin >> cdata.table_width;
    std::cout << "Enter table height in milimeters: ";
    std::cin >> cdata.table_height;
    cdata.dst[0] = {0, 0};
    cdata.dst[1] = { cdata.table_width, 0};
    cdata.dst[2] = { 0, cdata.table_height };
    cdata.dst[3] = { cdata.table_width, cdata.table_height };

show_warp:
    persp_mat = cv::getPerspectiveTransform(cdata.src, cdata.dst);
    while (true)
    {
        get_frame(video, frame);
        cv::warpPerspective(frame, wframe, persp_mat, cv::Size(cdata.table_width, cdata.table_height), cv::INTER_NEAREST);
        cv::resize(wframe, wframe, cv::Size(frame.rows * cdata.table_width / cdata.table_height, frame.rows), cv::INTER_NEAREST);
        draw_help(wframe, HELP_RESULT);
        cv::imshow(WINDOW_NAME, wframe);
        int key = cv::waitKey(10);
        if ('r' == key) {
            goto main_selection;
        }
        if ('s' == key) {
            cdata.save(SAVE_FILE_PATH);
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

inline
void get_frame(cv::VideoCapture& video, cv::Mat& frame)
{
    while (!video.read(frame)) {
        continue;
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

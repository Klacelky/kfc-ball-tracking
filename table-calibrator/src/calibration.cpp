#include "headers/calibration.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define WINDOW_NAME "Calibration"
#define CORNER_COLOR cv::Scalar(0, 0, 255)
#define CORNER_SIZE 5

void start_calibration(cv::VideoCapture video)
{
    cv::Mat frame;
    MouseData mdata;
    std::vector<cv::Point2f> points;

    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, mouse_controller, &mdata);

    while (true)
    {
        if (!video.read(frame)) {
            continue;
        }
        if (mdata.clicked) {
            points.push_back(cv::Point2f(mdata.x, mdata.y));
            mdata.clicked = false;
        }
        draw_points(frame, points);
        cv::imshow(WINDOW_NAME, frame);
        cv::waitKey(10);
    }
}

void mouse_controller(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN) {
        MouseData* mdata = static_cast<MouseData*>(userdata);
        mdata->x = x;
        mdata->y = y;
        mdata->clicked = true;
    }
}

void draw_points(cv::Mat frame, std::vector<cv::Point2f> points)
{
    for (int i = 0; i < points.size(); i++) {
        cv::circle(frame, points[i], CORNER_SIZE, CORNER_COLOR, cv::FILLED);
    }
}

#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


int main(int argc, char* argv[])
{
    cv::VideoCapture video;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-f") {
            if (++i >= argc) {
                std::cerr << "Error: Missing file path after \'-f\'.";
                return 1;
            }
            std::cout << argv[i] << std::endl;
            video = cv::VideoCapture(cv::String(argv[i]));
            continue;
        }
        if (arg == "-c") {
            if (++i >= argc) {
                std::cerr << "Error: Missing camera index after \'-c\'.";
                return 1;
            }
            video = cv::VideoCapture(atoi(argv[i]), cv::CAP_DSHOW);
            video.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
            video.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
            video.set(cv::CAP_PROP_FRAME_COUNT, 50);
            continue;
        }
        if (arg == "-h") {
            std::cout << "Usage: program [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -f <filename>   Specify video file to open." << std::endl;
            std::cout << "  -c <camera_id>  Specify camera ID to open." << std::endl;
            std::cout << "  -h              Show this help message." << std::endl;
        }
        std::cerr << "Error: Unknown option " << arg << std::endl;
        return 1;
    }

    if (!video.isOpened()) {
        std::cerr << "\nError: Unable to open video/camera.\n" << std::endl;
        return 1;
    }

    cv::Mat frame;

    while (true) {
        if (!video.read(frame)) {
            continue;
        }
        cv::waitKey(10);
        cv::imshow("Frame", frame);
    }

    video.release();

    return 0;
}

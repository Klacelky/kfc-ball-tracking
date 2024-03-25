#include <iostream>
#include "tracking.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Error: Video path not provided." << std::endl
            << "Usage: " << argv[0] << " <video_file>" << std::endl;
        return -1;
    }

    cv::VideoCapture video(argv[1]);

    if (!video.isOpened()) {
        std::cerr << "Error: Couldn't open file " << argv[1];
        return -1;
    }

    switch (Track(video)) {
        case ERROR_INTERRUPT:
            std::cerr
                << "main: Tracking interrupted by error." << std::endl;
            return -1;
        case USER_INTERRUPT:
            std::cerr << "main: Tracking interrupted by user." << std::endl;
            return -1;
        case EOF_REACHED:
            std::cout << "main: Tracking finished, EOF reached." << std::endl;
            break;
    }

    return 0;
}

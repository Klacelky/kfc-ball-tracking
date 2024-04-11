#include <iostream>
#include <opencv2/videoio.hpp>

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

    return 0;
}

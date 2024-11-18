#include "CameraFactory.h"
#include "opencv2/videoio.hpp"
#include <memory>
#include <spdlog/spdlog.h>

std::shared_ptr<cv::VideoCapture> CameraFactory::create(int index, int exposure) {
    std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>();
    capture->set(cv::CAP_PROP_EXPOSURE, exposure);
    capture->open(0);
    if (not capture->isOpened()) {
        spdlog::error("Failed to Open Camera {}", index);
    }
    return capture;
}
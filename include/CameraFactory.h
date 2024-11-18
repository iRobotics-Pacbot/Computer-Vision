#pragma once

#include <opencv2/videoio.hpp>

/**
 * @brief Constructs cameras
 * 
 */
struct CameraFactory {
    /**
     * @brief Constructs cameras based on index and exposure
     * 
     * @param index The desired index of the camera
     * @param exposure The desired exposure of the camera
     * @return The constructed camera
     */
    static std::shared_ptr<cv::VideoCapture> create(int index, int exposure);
};
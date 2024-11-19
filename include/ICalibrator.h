#pragma once

#include "opencv2/core/types.hpp"
#include <opencv2/core/mat.hpp>

/**
 * @brief Converts coordinates from the image to the maze grid
 * 
 */
struct ICalibrator {
    virtual ~ICalibrator() = default;
    /**
     * @brief Calibrates the calibrator
     * 
     * @param input The output of the camera
     * @return The subregion of interest
     */
    virtual void calibrate(cv::Mat &input) = 0;
    /**
     * @brief Fixes the perspective warping and crops the image
     * 
     * @param mat The input image
     */
    virtual void convert(cv::Mat& mat) const = 0;
};
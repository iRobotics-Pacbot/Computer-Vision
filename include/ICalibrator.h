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
    virtual cv::Rect calibrate(cv::Mat& input);
    /**
     * @brief Converts image space coordinates to maze grid space coordinates
     * 
     * @param position The image space coordinate
     * @return The grid space coordinate
     */
    virtual std::pair<int, int> convert(const std::pair<int, int>& position) const;
};
#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>

/**
 * @brief An image processing pipeline that finds the location of the marker
 * 
 */
struct IPipeline {
    virtual ~IPipeline() = default;
    /**
     * @brief Finds the location of the marker given a frame
     * 
     * @param mat The frame to process
     * @return The location of the marker
     */
    virtual std::pair<int, int> process(cv::Mat& mat) = 0;
};
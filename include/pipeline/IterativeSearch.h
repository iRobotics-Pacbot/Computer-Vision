#pragma once

#include "IPipeline.h"

/**
 * @brief Iteratively find the marker by locating boundaries
 * 
 */
struct IterativeSearch: public IPipeline {
    std::pair<int, int> process(cv::Mat& mat) override;
};
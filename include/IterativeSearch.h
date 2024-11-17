#pragma once
#include "IPipeline.h"

/*
A pipeline that uses an interative search to calculate the position of the marker by finding boundaries
*/
struct IterativeSearch: public IPipeline {
    std::pair<int, int> process(cv::Mat& mat) override;
};
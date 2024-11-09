#pragma once
#include "IPipeline.h"

struct IterativeSearch: public IPipeline {
    std::pair<int, int> process(cv::Mat& mat) override;
};
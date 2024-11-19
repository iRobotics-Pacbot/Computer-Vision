#pragma once
#include "ICalibrator.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"

/**
 * @brief Uses blob detection to calibrate
 * 
 */
class BlobCalibrator: public ICalibrator {
private:
    cv::Mat transformation;
public:
    void calibrate(cv::Mat& input) override;
    void convert(cv::Mat& mat) const override;
};
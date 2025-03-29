#pragma once
#include <opencv4/opencv2/core.hpp>

namespace CameraCalibration {

// Camera matrix
const cv::Mat CAMERA_MATRIX =
    (cv::Mat_<double>(3, 3) << 599.89070367, 0.0, 667.15479581, 0.0,
     602.51703451, 418.15479082, 0.0, 0.0, 1.0);

// Distortion coefficients
const cv::Mat DIST_COEFFS = (cv::Mat_<double>(1, 5) << 0.00809756, -0.01371771,
                             -0.00064726, 0.00014211, -0.00353749);

} // namespace CameraCalibration

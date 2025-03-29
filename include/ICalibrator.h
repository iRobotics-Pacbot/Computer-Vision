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
   * @brief Fixes the perspective warping and crops the image
   *
   * @param mat The input image
   */
  virtual void convert(cv::Mat &mat) const = 0;
};

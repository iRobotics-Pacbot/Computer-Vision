#include "pipeline/IterativeSearch.h"
#include <atomic>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

std::pair<int, int> IterativeSearch::process(cv::Mat &mat) {
  // Scale down the image for faster processing
  double scale = 0.25;
  cv::resize(mat, mat, {(int)(mat.cols * scale), (int)(mat.rows * scale)});

  // Turn to grayscale
  cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
  // cv::GaussianBlur(mat, mat, {7, 7}, 0);

  // Edges
  std::atomic_int leftMost{mat.cols};
  std::atomic_int rightMost{0};
  std::atomic_int topMost{mat.rows};
  std::atomic_int bottomMost{0};

  // Loop through and look for edges
  mat.forEach<uchar>([&](uchar &pixel, const int *position) {
    if (pixel > 255 / 2) {

      // Top
      if (position[0] < topMost) {
        topMost = position[0];
      }

      // Bottom
      if (position[0] > bottomMost) {
        bottomMost = position[0];
      }

      // Left
      if (position[1] < leftMost) {
        leftMost = position[1];
      }

      // Right
      if (position[1] > rightMost) {
        rightMost = position[1];
      }
    }
  });

  // Scale back up
  return {(topMost + bottomMost) / 2.0 / scale,
          (leftMost + rightMost) / 2.0 / scale};
}

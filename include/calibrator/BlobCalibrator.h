#pragma once
#include "ICalibrator.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/videoio.hpp"

class BlobCalibrator : public ICalibrator {
private:
  cv::Mat transformation;
  cv::VideoCapture cap;
  cv::Mat cameraMatrix, distCoeffs, newCameraMatrix;
  cv::Rect roi;

  static cv::Mat gammaTransform(const cv::Mat &img, double gamma);
  static double calcCircularity(double area, double perimeter);
  static double calcAspectRatio(double width, double height);
  static cv::Point2f pointPerspectiveTransform(const cv::Mat &matrix,
                                               const cv::Point2f &point);

public:
  BlobCalibrator();
  void convert(cv::Mat &mat) const override;
};

#include "calibrator/BlobCalibrator.h"
#include "calibrator/CalibrationParameters.h"
#include <cmath>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <spdlog/spdlog.h>

BlobCalibrator::BlobCalibrator() {
  // Use predefined camera matrix and distortion coefficients from the
  // CameraCalibration namespace
  cameraMatrix = CameraCalibration::CAMERA_MATRIX;
  distCoeffs = CameraCalibration::DIST_COEFFS;

  // Set the new camera matrix for undistortion
  cv::Size frameSize(1280, 800);
  newCameraMatrix = cv::getOptimalNewCameraMatrix(
      cameraMatrix, distCoeffs, frameSize, 1, frameSize, &roi);
}

cv::Mat BlobCalibrator::gammaTransform(const cv::Mat &img, double gamma) {
  cv::Mat lookupTable(1, 256, CV_8U);
  uchar *p = lookupTable.ptr();
  for (int i = 0; i < 256; ++i) {
    p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
  }

  cv::Mat result;
  cv::LUT(img, lookupTable, result);
  return result;
}

cv::Point2f
BlobCalibrator::pointPerspectiveTransform(const cv::Mat &matrix,
                                          const cv::Point2f &point) {
  // Perform a perspective transform on a point
  cv::Mat pt = (cv::Mat_<double>(3, 1) << point.x, point.y, 1.0);
  cv::Mat transformed = matrix * pt;
  return cv::Point2f(
      transformed.at<double>(0, 0) / transformed.at<double>(2, 0),
      transformed.at<double>(1, 0) / transformed.at<double>(2, 0));
}

void BlobCalibrator::convert(cv::Mat &mat) const {
  if (mat.empty()) {
    spdlog::error("Conversion input image is empty");
  }

  // Apply gamma correction if needed
  // You can decide to apply gamma correction here based on some condition,
  // like:
  mat = gammaTransform(mat, 3); // Example gamma value

  // Undistort the image using the stored calibration matrices
  cv::Mat undistorted;
  cv::undistort(mat, undistorted, cameraMatrix, distCoeffs, newCameraMatrix);
  undistorted = undistorted(roi);

  // Define source points from the calibration
  std::vector<cv::Point2f> srcPoints = {
      pointPerspectiveTransform(newCameraMatrix, cv::Point2f(0, 0)),
      pointPerspectiveTransform(newCameraMatrix,
                                cv::Point2f(undistorted.cols, 0)),
      pointPerspectiveTransform(
          newCameraMatrix, cv::Point2f(undistorted.cols, undistorted.rows)),
      pointPerspectiveTransform(newCameraMatrix,
                                cv::Point2f(0, undistorted.rows))};

  // Define destination points as a rectangle
  std::vector<cv::Point2f> dstPoints = {
      cv::Point2f(0, 0), cv::Point2f(undistorted.cols, 0),
      cv::Point2f(undistorted.cols, undistorted.rows),
      cv::Point2f(0, undistorted.rows)};

  // Compute the perspective transformation matrix
  cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);
  cv::warpPerspective(undistorted, mat, perspectiveMatrix, undistorted.size());
}

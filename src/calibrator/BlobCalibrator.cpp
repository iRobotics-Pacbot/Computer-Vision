#include "calibrator/BlobCalibrator.h"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/features2d.hpp>

void BlobCalibrator::calibrate(cv::Mat& input)  {
    // TODO: Create configuration files
    // Create the blob parameters to filter out circular shapes
    cv::SimpleBlobDetector::Params params;
    params.filterByCircularity = true;
    params.maxCircularity = 0.75;

    // Create the blob detector
    std::shared_ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

    // Get the keypoints
    std::vector<cv::KeyPoint> keyPoints;
    detector->detect(input, keyPoints);

    // Convert to the point array
    cv::Point2f markerPoints[4];
    for (int i = 0; i < 4; i++) {
        markerPoints[i] = keyPoints[i].pt;
    }

    // Sort by how far away from the top left
    std::sort(
        std::begin(markerPoints),
        std::end(markerPoints),
        [&](const cv::Point2d& a, const cv::Point2d& b) {
            return (a.x + a.y * input.cols) < (b.x + b.y * input.cols);
        }
    );

    // TODO: Check the coordinate order
    cv::Point2f realPositions[]{
        {0, 0},
        {0, (float) input.cols},
        {(float) input.rows, 0},
        {(float) input.rows, (float) input.cols}
    };
    transformation = cv::getPerspectiveTransform(markerPoints, realPositions);
}

void BlobCalibrator::convert(cv::Mat& input) const {

}

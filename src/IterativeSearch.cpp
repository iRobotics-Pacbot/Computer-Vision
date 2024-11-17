
#include "IterativeSearch.h"
#include <atomic>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

std::pair<int, int> IterativeSearch::process(cv::Mat& mat) {

    double scale = 0.25;
    cv::resize(mat, mat, {(int) (mat.cols * scale), (int) (mat.rows * scale)});
    cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
    //cv::GaussianBlur(mat, mat, {7, 7}, 0);

    std::atomic_int leftMost{mat.cols};
    std::atomic_int rightMost{0};
    std::atomic_int topMost{mat.rows};
    std::atomic_int bottomMost{0};

    mat.forEach<uchar>([&](uchar &pixel, const int * position) {
        if (pixel > 255/2) {

            if (position[0] < topMost) {
                topMost = position[0];
            }

            if (position[0] > bottomMost) {
                bottomMost = position[0];
            }
            if (position[1] < leftMost) {
                leftMost = position[1];
            }

            if (position[1] > rightMost) {
                rightMost = position[1];
            }
        }
    });

        
    return {(topMost + bottomMost)/2.0/scale, (leftMost + rightMost)/2.0/scale};
}
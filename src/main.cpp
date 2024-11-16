#include "IterativeSearch.h"
#include <chrono>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <opencv2/videoio.hpp>

int main() {
    std::filesystem::path path = "testImages/sample.png";
    cv::VideoCapture capture;
    capture.open(0, cv::CAP_ANY);
    cv::Mat image;
    IPipeline* pipeline = new IterativeSearch();

    cv::namedWindow("Live");
    
    while (true) {
        capture.read(image);
        cv::Mat copy = image.clone();
        std::pair<int, int> pos = pipeline->process(copy);
        cv::circle(image, {pos.first, pos.second}, 10, {0, 255, 0});
        cv::imshow("Live", image);
    }


    delete pipeline;
}
#include "IterativeSearch.h"
#include <chrono>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <filesystem>

int main() {
    std::filesystem::path path = "testImages";
    cv::Mat image = cv::imread(path/"sample2.png");

    cv::Mat copy = image.clone();
    IPipeline* pipeline = new IterativeSearch();

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::pair<int, int> pos = pipeline->process(copy);
    printf("Time (ms): %ld\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());

    cv::circle(image, {pos.second, pos.first}, 10, {255, 0 ,0});
    
    cv::namedWindow("Display");
    cv::imshow("Display", image);
    cv::waitKey(0);
    delete pipeline;
}
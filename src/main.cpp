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
    cv::Mat image = cv::imread(path/"sample.png");
    IPipeline* pipeline = new IterativeSearch();

    int samples = 1e4;
    long totalTime = 0;
    
    for (int i = 0; i < samples; i++) {
        cv::Mat copy = image.clone();
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        std::pair<int, int> pos = pipeline->process(copy);
        totalTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
    }
    printf("Time (micros): %ld\n", totalTime/samples);
    delete pipeline;
}
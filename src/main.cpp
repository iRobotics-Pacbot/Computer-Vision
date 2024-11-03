#include <opencv2/core/mat.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <filesystem>

int main() {
    std::filesystem::path path = "testImages";
    cv::Mat image = cv::imread(path/"sample.png");
    cv::namedWindow("Window");
    cv::imshow("Window", image);
    cv::waitKey(0);
}
#include "IterativeSearch.h"
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

int main() {
    // Gets the user's configuration
    int cam, exposure;
    std::cout << "Camera Index: ";
    std::cin >> cam;
    std::cout << "Exposure: ";
    std::cin >> exposure;

    // Starts up the camera
    cv::VideoCapture capture;
    capture.open(cam, cv::CAP_ANY);

    // Sets the camera exposure
    capture.set(cv::CAP_PROP_EXPOSURE, exposure);

    // Creates the buffer
    cv::Mat image;

    // Creates the pipeline
    std::unique_ptr<IPipeline> pipeline = std::make_unique<IterativeSearch>();

    // Creates the window
    cv::namedWindow("Live");

    // Runs while the 
    while (cv::waitKey(1) < 0) {
        // Grab the frame
        capture.read(image);

        // Copy the frame so it can be used for display later
        cv::Mat copy = image.clone();

        // Get the coordinates of the marker
        std::pair<int, int> pos = pipeline->process(copy);

        // Draw a circle around the marker
        cv::circle(image, {pos.second, pos.first}, 10, {0, 255, 0});

        //Display image to the window
        cv::imshow("Live", image);
    }
}
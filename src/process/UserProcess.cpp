#include "process/UserProcess.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

void UserProcess::run(
    const std::shared_ptr<cv::VideoCapture>& camera,
    const std::shared_ptr<IPipeline>& pipeline,
    const std::shared_ptr<ICalibrator>& calibrator
) {
    // Buffer for the camera output
    cv::Mat image;

    // Window to display the camera output
    cv::namedWindow("Live");

    // Runs while the 
    while (cv::waitKey(1) < 0) {
        // Grab the frame
        camera->read(image);

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

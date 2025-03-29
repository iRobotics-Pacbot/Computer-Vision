#include "process/UserProcess.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

void UserProcess::run(const std::shared_ptr<cv::VideoCapture> &camera,
                      const std::shared_ptr<IPipeline> &pipeline,
                      const std::shared_ptr<ICalibrator> &calibrator) {
  const std::string saveFolder = "outputs";
  // Buffer for the camera output
  cv::Mat image;

  // Window to display the camera output
  cv::namedWindow("User Process");

  // Check if directory exists
  if (not std::filesystem::exists(saveFolder)) {
    // Create directory
    std::filesystem::create_directory(saveFolder);
  }

  // Runs while the
  for (int i = 0; cv::waitKey(1) < 0; ++i) {
    // Grab the frame
    camera->read(image);

    // Copy the frame so it can be used for display later
    cv::Mat copy = image.clone();

    // Get the coordinates of the marker
    std::pair<int, int> pos = pipeline->process(copy);

    // Draw a circle around the marker
    cv::circle(image, {pos.second, pos.first}, 10, {0, 255, 0});

    // Display image to the window
    cv::imshow("User Process", image);

    // Save the image
    cv::imwrite(std::filesystem::path(saveFolder) /
                    (std::to_string(i) + ".jpg"),
                image);
  }
}

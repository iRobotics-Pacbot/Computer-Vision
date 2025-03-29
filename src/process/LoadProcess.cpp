#include "process/LoadProcess.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <stdexcept>

void LoadProcess::run(const std::shared_ptr<cv::VideoCapture> &camera,
                      const std::shared_ptr<IPipeline> &pipeline,
                      const std::shared_ptr<ICalibrator> &calibrator) {
  const std::string saveFolder = "outputs";
  // Buffer for the camera output
  cv::Mat image;

  // Window to display the camera output
  cv::namedWindow("User Process");

  // Check if directory exists
  if (not std::filesystem::exists(saveFolder)) {
    // Fail
    spdlog::critical("Could not find saves folder");
    throw std::runtime_error("No save folder");
  }

  std::filesystem::directory_iterator directoryIterator =
      std::filesystem::directory_iterator(saveFolder);

  // Runs while the
  for (const std::filesystem::directory_entry &entry : directoryIterator) {
    spdlog::debug("Getting frame {}", entry.path().string());

    // Grab the frame
    image = cv::imread(entry.path());

    // Copy the frame so it can be used for display later
    cv::Mat copy = image.clone();

    // Get the coordinates of the marker
    std::pair<int, int> pos = pipeline->process(copy);

    // Draw a circle around the marker
    cv::circle(image, {pos.second, pos.first}, 10, {0, 255, 0});

    // Display image to the window
    cv::imshow("User Process", image);

    // Save the image
    cv::waitKey(0);
  }
}

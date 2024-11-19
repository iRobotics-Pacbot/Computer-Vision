#include "CalibratorFactory.h"
#include "CameraFactory.h"
#include "Config.h"
#include "IPipeline.h"
#include "IProcess.h"
#include "PipelineFactory.h"
#include "ProcessFactory.h"
#include "opencv2/videoio.hpp"
#include "spdlog/common.h"
#include <spdlog/spdlog.h>
#include <memory>
#include <filesystem>

/**
 * @brief Computer Vision (CV) client for PacBots competition
 * 
 * @param argc The number of command line arguments
 * @param argv  The command line arguments
 * @return Exit code
 */
int main(int argc, char** argv) {
    // Set log level to see traces
    spdlog::set_level(spdlog::level::trace);

    spdlog::info("Starting Program");

    // Set the working directory to the binary parent directory
    std::filesystem::path path = std::filesystem::path(argv[0]).parent_path();
    std::filesystem::current_path(path);
    spdlog::debug("Path {}", path.string());

    // Read the configurations for the CV Client
    spdlog::info("Loading Configurations");
    Config config;

    // Read the default.yaml and override it with the user.yaml
    config.loadFromFile("default.yaml");
    config.loadFromFile("user.yaml");

    // Construct the pipeline
    spdlog::info("Creating Pipeline");
    std::shared_ptr<IPipeline> pipeline = PipelineFactory::create(config.pipeline);

    // Construct the process
    spdlog::info("Creating Process");
    std::shared_ptr<IProcess> process = ProcessFactory::create(config.process);

    // Construct the calibrator
    spdlog::info("Creating Calibrator");
    std::shared_ptr<ICalibrator> calibrator = CalibratorFactory::create(config.calibrator);

    // Construct the camera
    spdlog::info("Creating Camera");
    std::shared_ptr<cv::VideoCapture> camera = CameraFactory::create(config.cameraIndex, config.exposure);

    // Run the process
    spdlog::info("Running");
    process->run(camera, pipeline, calibrator);

    // Release the camera
    spdlog::info("Releasing Camera");
    camera->release();

    spdlog::info("Program Terminated");
    return 0;
}
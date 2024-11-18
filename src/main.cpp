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

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting Program");

    spdlog::info("Loading Configurations");
    Config config;
    std::filesystem::path path = std::filesystem::path(argv[0]).parent_path();
    spdlog::debug("Path {}", path.string());
    config.loadFromFile((path / "default.yaml").string());
    config.loadFromFile((path / "user.yaml").string());

    spdlog::info("Creating Pipeline");
    std::shared_ptr<IPipeline> pipeline = PipelineFactory::create(config.pipeline);

    spdlog::info("Creating Process");
    std::shared_ptr<IProcess> process = ProcessFactory::create(config.process);

    spdlog::info("Creating Camera");
    std::shared_ptr<cv::VideoCapture> camera = CameraFactory::create(config.cameraIndex, config.exposure);

    spdlog::info("Running");
    process->run(camera, pipeline);

    spdlog::info("Releasing Camera");
    camera->release();

    spdlog::info("Program Terminated");
}
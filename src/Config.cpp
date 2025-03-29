#include "Config.h"
#include "yaml-cpp/exceptions.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

void Config::loadFromFile(const std::string &path) {
  // Load the yaml data and override configurations
  try {
    YAML::Node data = YAML::LoadFile(path);

    if (data["process"]) {
      spdlog::debug("Process {}", process = data["process"].as<std::string>());
    }

    if (data["pipeline"]) {
      spdlog::debug("Pipeline {}",
                    pipeline = data["pipeline"].as<std::string>());
    }

    if (data["calibrator"]) {
      spdlog::debug("Calibrator {}",
                    calibrator = data["calibrator"].as<std::string>());
    }

    if (data["cameraIndex"]) {
      spdlog::debug("CameraIndex {}",
                    cameraIndex = data["cameraIndex"].as<int>());
    }

    if (data["exposure"]) {
      spdlog::debug("Exposure {}", exposure = data["exposure"].as<int>());
    }
  } catch (YAML::ParserException &e) {
    spdlog::critical("Failed to read yaml: {}", path);
    throw e;
  }
}

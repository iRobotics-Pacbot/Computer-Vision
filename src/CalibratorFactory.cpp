#include "CalibratorFactory.h"
#include "calibrator/BlobCalibrator.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>

std::shared_ptr<ICalibrator>
CalibratorFactory::create(const std::string &name) {
  if (name == "blob") {
    return std::make_shared<BlobCalibrator>();
  } else {
    spdlog::critical("Unknown Calibrator Type {}", name);
    throw std::runtime_error("Unknown Calibrator Type");
  }
  return nullptr;
}

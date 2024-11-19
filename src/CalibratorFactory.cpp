#include "CalibratorFactory.h"
#include <memory>
#include <spdlog/spdlog.h>

std::shared_ptr<ICalibrator> CalibratorFactory::create(const std::string& name) {
    // TODO: Add options for calibrators
    spdlog::error("Unkown Calibrator Type {}", name);
    return nullptr;
}
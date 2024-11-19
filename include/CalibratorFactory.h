#pragma once

#include "ICalibrator.h"
#include <memory>

/**
 * @brief Constructs calibrators
 * 
 */
struct CalibratorFactory {
    /**
     * @brief Constructs a calibrator based on name
     * 
     * @param name The type of calibrator
     * @return The constructed calibrator
     */
    static std::shared_ptr<ICalibrator> create(const std::string& name);
};
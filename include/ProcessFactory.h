#pragma once

#include "IProcess.h"
#include <memory>

/**
 * @brief Constructs processes
 * 
 */
struct ProcessFactory {
    /**
     * @brief Constructs a process based on the name
     * 
     * @param name The process type
     * @return The process created
     */
    static std::shared_ptr<IProcess> create(const std::string& name);
};
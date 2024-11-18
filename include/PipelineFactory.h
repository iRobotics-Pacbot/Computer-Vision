#pragma once

#include "IPipeline.h"
#include <memory>

/**
 * @brief Constructs pipelines
 * 
 */
#include <memory>
struct PipelineFactory {
    /**
     * @brief Constructs a pipeline based on the name
     * 
     * @param name The pipeline type
     * @return The pipeline created
     */
    static std::shared_ptr<IPipeline> create(const std::string& name);
};
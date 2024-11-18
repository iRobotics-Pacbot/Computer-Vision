#include "PipelineFactory.h"
#include "pipeline/IterativeSearch.h"
#include <memory>
#include <spdlog/spdlog.h>

std::shared_ptr<IPipeline> PipelineFactory::create(const std::string& name) {
    if (name == "iterative") {
        return std::make_shared<IterativeSearch>();
    } else {
        spdlog::error("Unkown Pipeline Type {}", name);
    }
    return nullptr;
}
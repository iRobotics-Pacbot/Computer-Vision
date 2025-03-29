#include "PipelineFactory.h"
#include "pipeline/IterativeSearch.h"
#include <memory>
#include <spdlog/spdlog.h>

std::shared_ptr<IPipeline> PipelineFactory::create(const std::string &name) {
  if (name == "iterative") {
    return std::make_shared<IterativeSearch>();
  } else {
    spdlog::critical("Unkown Pipeline Type {}", name);
    throw std::runtime_error("Unknown Pipeline Type");
  }
  return nullptr;
}

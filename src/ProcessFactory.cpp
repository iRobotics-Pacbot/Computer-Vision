#include "ProcessFactory.h"
#include "IProcess.h"
#include "process/ServerProcess.h"
#include "process/UserProcess.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>

std::shared_ptr<IProcess> ProcessFactory::create(const std::string &name) {
  if (name == "user") {
    return std::make_shared<UserProcess>();
  } else if (name == "server") {
    return std::make_shared<ServerProcess>();
  } else {
    spdlog::critical("Unknown Process Type {}", name);
    throw std::runtime_error("Unknown Process Type");
  }
  return nullptr;
}

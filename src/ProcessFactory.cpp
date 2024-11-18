#include "ProcessFactory.h"
#include "IProcess.h"
#include "process/UserProcess.h"
#include <memory>
#include <spdlog/spdlog.h>

std::shared_ptr<IProcess> ProcessFactory::create(const std::string& name) {
    if (name == "user") {
        return std::make_shared<UserProcess>();
    } else {
        spdlog::error("Unkown Process Type {}", name);
    }
    return nullptr;
}
#include "process/ServerProcess.h"
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocketInitResult.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <format>

ServerProcess::ServerProcess() {
    ix::initNetSystem();
    spdlog::debug("Reading Server config.json");
    std::ifstream fileStream(std::filesystem::current_path().parent_path()/"config.json");
    if (not fileStream.is_open()) {
        spdlog::error("Failed to Open Server config.json");
    }
    nlohmann::json data = nlohmann::json::parse(fileStream);
    fileStream.close();

    std::string address = std::format("ws://{}:{}/", data["ServerIP"].get<std::string>(), data["WebSocketPort"].get<int>());

    spdlog::info("Connecting to {}", address);

    socket = std::make_unique<ix::WebSocket>();
    socket->setUrl(address);
    socket->setPingInterval(45);

    socket->setOnMessageCallback([](const auto& _){});

    ix::WebSocketInitResult result = socket->connect(10);
    if (not result.success) {
        spdlog::error("Failed to connect to server");
        spdlog::trace(result.errorStr);
    }
}

ServerProcess::~ServerProcess() {
    ix::uninitNetSystem();
}

void ServerProcess::run(const std::shared_ptr<cv::VideoCapture>& camera, const std::shared_ptr<IPipeline>& pipeline) {
    cv::Mat frame;
    char data[sizeof(char) + 2 * sizeof(int)];
    data[0] = 'x';

    while (socket->getReadyState() == ix::ReadyState::Open) {
        camera->read(frame);
        std::pair<int, int> pos = pipeline->process(frame);
        std::memcpy(data + sizeof(char), (char*) &pos.first, sizeof(int));
        std::memcpy(data + sizeof(char) + sizeof(int), (char*) &pos.second, sizeof(int));
        socket->sendBinary(data);
    }

    socket->stop();
    spdlog::info("Client Disconnected");
}
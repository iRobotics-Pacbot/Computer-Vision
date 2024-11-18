#include "process/ServerProcess.h"
#include "nlohmann/json_fwd.hpp"
#include <asio.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <format>
#include <opencv2/core.hpp>

ServerProcess::ServerProcess() {
    spdlog::debug("Reading Server config.json");
    std::ifstream fileStream(std::filesystem::current_path().parent_path()/"config.json");
    if (not fileStream.is_open()) {
        spdlog::error("Failed to Open Server config.json");
    }
    nlohmann::json data = nlohmann::json::parse(fileStream);
    fileStream.close();

    std::string host = data["ServerIP"].get<std::string>();
    std::string port = std::to_string(data["WebSocketPort"].get<int>());
    
    spdlog::info("Connecting to ws://{}:{}", host, port);

    context = std::make_unique<asio::io_context>();
    asio::ip::tcp::resolver resolver(*context);
    socket = std::make_unique<asio::ip::tcp::socket>(*context);
    socket->connect(resolver.resolve(host, port)->endpoint());

    webThread = std::make_unique<std::thread>([&]() {
        while (socket->is_open()) {
            context->run();
        }
    });
}

void ServerProcess::run(const std::shared_ptr<cv::VideoCapture>& camera, const std::shared_ptr<IPipeline>& pipeline) {
    cv::Mat frame;
    char data[sizeof(char) + 2 * sizeof(int)];
    data[0] = 'x';

    while (socket->is_open()) {
        socket->async_receive(nullptr);
        camera->read(frame);
        std::pair<int, int> pos = pipeline->process(frame);
        std::memcpy(data + sizeof(char), (char*) &pos.first, sizeof(int));
        std::memcpy(data + sizeof(char) + sizeof(int), (char*) &pos.second, sizeof(int));
        socket->async_send(data);
    }
    webThread->join();

    spdlog::info("Client Disconnected");
}
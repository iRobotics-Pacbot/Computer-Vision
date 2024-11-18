#pragma once

#include "IProcess.h"
#include "asio/io_context.hpp"
#include <asio.hpp>
#include <thread>

/**
 * @brief A process that sends the location data to the server
 * 
 */
class ServerProcess: public IProcess {
private:
    std::unique_ptr<asio::io_context> context;
    std::unique_ptr<asio::ip::tcp::socket> socket;
    std::unique_ptr<std::thread> webThread;
public:
    ServerProcess();
    void run(const std::shared_ptr<cv::VideoCapture>& camera, const std::shared_ptr<IPipeline>& pipeline) override;
};
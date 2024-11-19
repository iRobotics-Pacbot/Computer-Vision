#pragma once

#include <memory>
#include "IProcess.h"
#include <ixwebsocket/IXWebSocket.h>

/**
 * @brief A process that sends the location data to the server
 * 
 */
class ServerProcess: public IProcess {
private:
    std::unique_ptr<ix::WebSocket> socket;
public:
    ServerProcess();
    ~ServerProcess() override;
    void run(
        const std::shared_ptr<cv::VideoCapture>& camera,
        const std::shared_ptr<IPipeline>& pipeline,
        const std::shared_ptr<ICalibrator>& calibrator
    ) override;
};
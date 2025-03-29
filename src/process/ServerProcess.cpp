#include "process/ServerProcess.h"
#include "opencv2/core/mat.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketInitResult.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <spdlog/spdlog.h>

ServerProcess::ServerProcess() {
  // Initialize the websocket system
  ix::initNetSystem();

  // Read configurations out of the config.json
  spdlog::debug("Reading Server config.json");
  std::ifstream fileStream(std::filesystem::current_path().parent_path() /
                           "config.json");
  if (not fileStream.is_open()) {
    spdlog::error("Failed to Open Server config.json");
  }
  nlohmann::json data = nlohmann::json::parse(fileStream);
  fileStream.close();

  // Create the address
  std::string address =
      std::format("ws://{}:{}/", data["ServerIP"].get<std::string>(),
                  data["WebSocketPort"].get<int>());

  spdlog::info("Connecting to {}", address);

  // Create the websocket and configure parameters
  socket = std::make_unique<ix::WebSocket>();
  socket->setUrl(address);
  socket->setPingInterval(45);

  // A black callback to ignore messages from the server
  socket->setOnMessageCallback([](const auto &_) {});

  // Attempt to connect to the server
  ix::WebSocketInitResult result = socket->connect(10);
  if (not result.success) {
    spdlog::error("Failed to connect to server");
    spdlog::trace(result.errorStr);
  }
}

ServerProcess::~ServerProcess() {
  // Shutdown the websocket system
  ix::uninitNetSystem();
}

void ServerProcess::run(const std::shared_ptr<cv::VideoCapture> &camera,
                        const std::shared_ptr<IPipeline> &pipeline,
                        const std::shared_ptr<ICalibrator> &calibrator) {
  // Buffer for the camera output
  cv::Mat frame;

  // Flag to check if the calibrator has been calibrated

  // Buffer for sending to the server
  char data[sizeof(char) + 2 * sizeof(int)];

  // Indicate that this is from the CV Client
  data[0] = 'x';

  // Run the process while connected to the server
  while (socket->getReadyState() == ix::ReadyState::Open) {
    // Grab the frame from the camera
    camera->read(frame);

    // Check to see if there is a frame
    if (frame.empty()) {
      spdlog::error("Failed to capture frame from camera");

      // Skip this frame
      continue;
    }

    // Transform the frame
    try {
      calibrator->convert(frame);
    } catch (const std::exception &e) {
      spdlog::error("Failed to apply calibration: {}", e.what());

      // Skip this frame
      continue;
    }

    // Get the position using the pipeline
    std::pair<int, int> pos = pipeline->process(frame);

    // Copy the position data into the buffer
    std::memcpy(data + sizeof(char), (char *)&pos.first, sizeof(int));
    std::memcpy(data + sizeof(char) + sizeof(int), (char *)&pos.second,
                sizeof(int));

    // Send the position data to the server
    socket->sendBinary(data);
  }

  // Stop the websocket
  socket->stop();
  spdlog::info("Client Disconnected");
}

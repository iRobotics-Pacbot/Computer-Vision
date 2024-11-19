#pragma once

#include <string>

// TODO: Marked for deletion
/**
 * @brief Stores the configration for the program
 * 
 */
struct Config {
    std::string process;
    std::string pipeline;
    std::string calibrator;
    int cameraIndex;
    int exposure;

    /**
     * @brief Loads the configuration from a path
     * 
     * @param path The path
     */
    void loadFromFile(const std::string& path);
};
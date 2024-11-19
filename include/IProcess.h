#pragma once

#include "ICalibrator.h"
#include "IPipeline.h"
#include "opencv2/videoio.hpp"
#include <memory>

/**
 * @brief Represents a program that runs off of the camera and pipeline
 * 
 */
struct IProcess {
    virtual ~IProcess() = default;
    /**
     * @brief Runs the pipeline
     * 
     * @param camera The camera to be used
     * @param pipeline The pipeline to be used
     */
    virtual void run(
        const std::shared_ptr<cv::VideoCapture>& camera,
        const std::shared_ptr<IPipeline>& pipeline,
        const std::shared_ptr<ICalibrator>& calibrator
    ) = 0;
};
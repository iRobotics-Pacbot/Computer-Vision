#pragma once
#include "IProcess.h"

/**
 * @brief A process for the user to see the pipeline output
 * 
 */
struct UserProcess: public IProcess {
    void run(
        const std::shared_ptr<cv::VideoCapture>& camera,
        const std::shared_ptr<IPipeline>& pipeline,
        const std::shared_ptr<ICalibrator>& calibrator
    ) override;
};
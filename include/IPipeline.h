#include <opencv2/core/mat.hpp>
#include <opencv4/opencv2/core.hpp>

class IPipeline {
    virtual ~IPipeline() = 0;
    virtual std::pair<int, int> process(cv::Mat& mat);
};
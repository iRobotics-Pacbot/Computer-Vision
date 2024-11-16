#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>

struct IPipeline {
    virtual ~IPipeline() = default;
    virtual std::pair<int, int> process(cv::Mat& mat) = 0;
};
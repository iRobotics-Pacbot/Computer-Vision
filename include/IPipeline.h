#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>

/*
This interface represents a processing pipeline
*/
struct IPipeline {
    virtual ~IPipeline() = default;
    /*
    Calculates the coordinates of the marker based on the image
    @param mat The image to be processed
    @return The location of the marker on the image
    */
    virtual std::pair<int, int> process(cv::Mat& mat) = 0;
};